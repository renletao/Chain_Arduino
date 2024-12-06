/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "ChainCommon.h"

/**
 * @brief Mutex semaphore to control access.
 */
static bool mutexLocked = false;

// 锁定互斥量
bool ChainCommon::acquireMutex(void)
{
    if (!mutexLocked) {
        mutexLocked = true;
        return true;
    }
    return false;
}

// 释放互斥量
void ChainCommon::releaseMutex(void)
{
    mutexLocked = false;
}

void ChainCommon::begin(HardwareSerial *serial, unsigned long baud, int8_t rxPin, int8_t txPin, uint32_t config,
                        bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
{
    serialPort = serial;
    serialPort->begin(baud, SERIAL_8N1, rxPin, txPin, invert, timeout_ms, rxfifo_full_thrhd);
}
// 检查数据包头
bool ChainCommon::checkPacketHeader(const uint8_t *buffer, uint16_t size)
{
    return (size >= 2 && buffer[0] == PACK_HEAD_HIGH && buffer[1] == PACK_HEAD_LOW);
}

// 检查数据包尾
bool ChainCommon::checkPacketTail(const uint8_t *buffer, uint16_t size)
{
    return (size >= 2 && buffer[size - 1] == PACK_END_LOW && buffer[size - 2] == PACK_END_HIGH);
}

// 检查数据包的 CRC 校验
bool ChainCommon::checkCRC(const uint8_t *buffer, uint16_t size)
{
    uint8_t crc8 = calculateCRC(buffer, size);
    return (crc8 == buffer[size - 3]);
}

// 检查数据包的长度
bool ChainCommon::checkPacketLength(const uint8_t *buffer, uint16_t size)
{
    uint16_t length = (uint16_t)(buffer[3] << 8) | buffer[2];
    return ((length + 6) == size);
}

// 计算 CRC 校验值
uint8_t ChainCommon::calculateCRC(const uint8_t *buffer, uint16_t size)
{
    uint8_t crc8 = 0;
    for (uint8_t i = 4; i < (size - 3); i++) {
        crc8 += buffer[i];
    }
    return crc8;
}

// 发送数据包
void ChainCommon::sendPacket(uint16_t id, uint8_t cmd, const uint8_t *buffer, uint16_t size)
{
    memset(sendBuffer, 0, sendBufferSize);  // 清除缓冲
    uint8_t crc8     = 0;
    uint16_t cmdSize = size + 3;
    sendBufferSize   = size + 9;
    sendBuffer[0]    = PACK_HEAD_HIGH;
    sendBuffer[1]    = PACK_HEAD_LOW;
    sendBuffer[2]    = cmdSize & 0xFF;
    sendBuffer[3]    = (cmdSize >> 8) & 0xFF;
    sendBuffer[4]    = id;
    sendBuffer[5]    = cmd;
    memcpy(sendBuffer + 6, buffer, size);
    crc8                           = calculateCRC(sendBuffer, sendBufferSize);
    sendBuffer[sendBufferSize - 3] = crc8;
    sendBuffer[sendBufferSize - 2] = PACK_END_HIGH;
    sendBuffer[sendBufferSize - 1] = PACK_END_LOW;
    if (serialPort) {
        serialPort->write(reinterpret_cast<const char *>(sendBuffer), sendBufferSize);
    }
}

// 辅助函数：处理包的提取、头尾检查和数据清理
bool ChainCommon::processPacketData(uint16_t id, uint8_t cmd)
{
    uint16_t startIndex = 0;
    bool packetFound    = false;  // 标记是否找到符合条件的数据包

    while (startIndex < receiveBufferSize) {
        // 找到数据包的开始标志 0xAA 0x55
        if (receiveBuffer[startIndex] == 0xAA && receiveBuffer[startIndex + 1] == 0x55) {
            // 数据包的长度字节
            if (startIndex + 3 >= receiveBufferSize) {
                return packetFound;
            }

            // 获取数据包长度
            uint16_t length = receiveBuffer[startIndex + 2] | (receiveBuffer[startIndex + 3] << 8);

            // 确保包尾正确并且缓冲区剩余足够长度
            if (startIndex + 4 + length + 2 <= receiveBufferSize && receiveBuffer[startIndex + 4 + length] == 0x55 &&
                receiveBuffer[startIndex + 4 + length + 1] == 0xAA) {
                // 包头、包尾、长度加数据内容
                uint16_t packetSize = 4 + length + 2;  // 包头（2字节） + 长度（2字节） + 数据内容 + 包尾（2字节）

                // 直接处理缓冲区中的数据，不分配新内存
                uint8_t *packetData = receiveBuffer + startIndex;  // 数据包从 startIndex 开始

                // 更新缓冲区大小
                uint16_t remainingSize = receiveBufferSize - packetSize;
                receiveBufferSize      = remainingSize;

                uint8_t packetId    = packetData[4];
                uint8_t packetCmd   = packetData[5];
                uint8_t packetData6 = packetData[6];

                if (packetId == id && packetCmd == cmd) {
                    memcpy(cmdReturnBuffer, packetData, packetSize);
                    cmdReturnBufferSize = packetSize;
                    packetFound         = true;  // 标记找到符合条件的数据包
                } else if (packetCmd == CHAIN_ENUM_PLEASE &&
                           checkCRC(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                    enumPleaseCount++;  // 计数器增加
                } else if (packetCmd == 0x40 && packetData6 == 0x11 &&
                           checkCRC(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                    if (keyBufferSize < KEY_BUFFER_SIZE) {
                        keyBuffer[keyBufferSize++] = packetId;
                    }
                }

                // 如果缓冲区还有未处理的数据，继续处理
                if (remainingSize > 0) {
                    // 直接从下一个数据包的开始位置继续处理
                    memmove(receiveBuffer, receiveBuffer + startIndex + packetSize, remainingSize);
                    startIndex = 0;  // 从新的数据开始处理
                } else {
                    // 没有剩余数据，处理结束
                    break;
                }
            } else {
                break;  // 如果包尾不匹配或长度不足，跳出循环
            }
        } else {
            startIndex++;  // 如果当前不是包头，继续向后查找
        }
    }

    // 清除缓冲区和大小
    memset(receiveBuffer, 0, sizeof(receiveBuffer));  // 清空缓冲区
    receiveBufferSize = 0;                            // 重置缓冲区大小
    return packetFound;
}
bool ChainCommon::processPacket(uint16_t id, uint8_t cmd)
{
    return processPacketData(cmd, id);
}
void ChainCommon::processIncomingPacket(void)
{
    if (available() > 0) {
        receive();
        processPacketData();
    }
}
bool ChainCommon::checkPacket(const uint8_t *buffer, uint16_t size)
{
    return (checkPacketHeader(buffer, size) && checkPacketTail(buffer, size) && checkCRC(buffer, size) &&
            checkPacketLength(buffer, size));
}

bool ChainCommon::available(void)
{
    return serialPort->available() > 0;
}
// 取出数据
void ChainCommon::receive(void)
{
    unsigned long startTime = millis();

    while (true)  // 无限循环
    {
        // 检查超时
        if (millis() - startTime >= TIMEOUT_MS) {
            break;  // 超时后退出循环
        }

        // 检查是否有可用数据
        while (available()) {
            if (receiveBufferSize < sizeof(receiveBuffer)) {  // 防止越界
                receiveBuffer[receiveBufferSize++] = serialPort->read();
                startTime                          = millis();
            } else {
                break;
            }
        }
    }
}

bool ChainCommon::waitForData(uint16_t id, uint8_t cmd, uint32_t timeout)
{
    uint32_t startTime = millis();  // 获取当前时间
    while (millis() - startTime < timeout) {
        // 检查是否收到数据
        if (available()) {
            receive();
            bool status = processPacket(cmd, id);
            if (status) {
                return true;
            }
        }
        delay(1);
    }
    return false;
}

uint16_t ChainCommon::getEnumPleaseNum(void)
{
    uint16_t temp   = enumPleaseCount;
    enumPleaseCount = 0;
    return temp;
}

void ChainCommon::getKeyBuffer(uint16_t *outBuffer, uint16_t *length)
{
    *length = keyBufferSize;  // 返回当前keyBuffer的有效数据长度

    // 复制keyBuffer的内容到outBuffer
    memcpy(outBuffer, keyBuffer, keyBufferSize * sizeof(uint16_t));

    // 清空keyBuffer
    memset(keyBuffer, 0, KEY_BUFFER_SIZE);  // 清空keyBuffer数组
    keyBufferSize = 0;                      // 重置keyBufferSize
}

chain_status_t ChainCommon::setRGBValue(uint16_t id, rgb_color rgb, uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = rgb.R;
        cmdBuffer[cmdBufferSize++] = rgb.G;
        cmdBuffer[cmdBufferSize++] = rgb.B;
        sendPacket(id, CHAIN_SET_RGB_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SET_RGB_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getRGBValue(uint16_t id, rgb_color *rgb, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_RGB_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_RGB_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                // *bootloaderVersion = cmdReturnBuffer[6];
                rgb->R = cmdReturnBuffer[6];
                rgb->G = cmdReturnBuffer[7];
                rgb->B = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::setRGBLight(uint16_t id, uint8_t rgbBrightness, uint8_t *operationStatus,
                                        uint8_t saveToFlash, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = rgbBrightness;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_SET_RGB_LIGHT, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SET_RGB_LIGHT, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                *operationStatus = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getRGBLight(uint16_t id, uint8_t *rgbBrightness, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_RGB_LIGHT, cmdBuffer, cmdBufferSize);
        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_RGB_LIGHT, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *rgbBrightness = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getBootloaderVersion(uint16_t id, uint8_t *bootloaderVersion, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_GET_BOOTLOADER_VERSION, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                *bootloaderVersion = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getFirmwareVersion(uint16_t id, uint8_t *firmwareVersion, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_VERSION_DEVICE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_VERSION_DEVICE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *firmwareVersion = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getDeviceType(uint16_t id, uint16_t *deviceType, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_DEVICE_TYPE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_DEVICE_TYPE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *deviceType = (cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainCommon::getDeviceNum(uint16_t *deviceNum, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = 0x00;
        sendPacket(0xFF, CHAIN_ENUM, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(0xFF, CHAIN_ENUM, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *deviceNum = cmdReturnBuffer[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}

// 检查是否有设备连接
// 根据传参的次数，询问是否有设备连接
chain_status_t ChainCommon::isDeviceConnected(uint8_t maxRetries, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    int retryCount        = 0;
    if (acquireMutex()) {
        while (retryCount < maxRetries) {
            // 发送数据包
            memset(cmdBuffer, 0, cmdBufferSize);
            cmdBufferSize = 0;
            sendPacket(0xFF, CHAIN_HEARTBEAT, cmdBuffer, cmdBufferSize);

            // 等待接收数据
            if (waitForData(0xFF, CHAIN_HEARTBEAT, timeout)) {
                // 校验数据包
                if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                    break;
                }
            }
            retryCount++;  // 增加重试次数
        }
        if (retryCount == maxRetries) {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}

// 获取连接设备的类型
chain_status_t ChainCommon::getDeviceList(device_list_t *list, unsigned long timeout)
{
    chain_status_t status   = CHAIN_OK;
    unsigned long startTime = millis();
    for (uint16_t i = 0; i < list->count; i++) {
        list->devices[i].id = i + 1;
        status              = getDeviceType(i + 1, &list->devices[i].device_type, timeout);
        if (status != CHAIN_OK) {
            break;
        }
    }
    return status;
}
