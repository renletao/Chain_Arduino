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
bool ChainCommon::acquireMutex(void)
{
    if (!mutexLocked) {
        mutexLocked = true;
        return true;
    }
    return false;
}
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
uint8_t ChainCommon::calculateCRC(const uint8_t *buffer, uint16_t size)
{
    uint8_t crc8 = 0;
    for (uint8_t i = 4; i < (size - 3); i++) {
        crc8 += buffer[i];
    }
    return crc8;
}

bool ChainCommon::checkPacket(const uint8_t *buffer, uint16_t size)
{
    uint8_t crc8    = calculateCRC(buffer, size);
    uint16_t length = (uint16_t)(buffer[3] << 8) | buffer[2] + 6;
    if (size < PACK_SIZE_MIN) {
        return false;
    }
    if (buffer[0] != PACK_HEAD_HIGH || buffer[1] != PACK_HEAD_LOW || buffer[size - 1] != PACK_END_LOW ||
        buffer[size - 2] != PACK_END_HIGH) {
        return false;
    }
    if (length != size) {
        return false;
    }
    if (crc8 != buffer[size - 3]) {
        return false;
    }
    return true;
}
void ChainCommon::sendPacket(uint16_t id, uint8_t cmd, const uint8_t *buffer, uint16_t size)
{
    memset(sendBuffer, 0, sendBufferSize);
    uint8_t crc8     = 0;
    uint16_t cmdSize = size + 3;
    sendBufferSize   = size + 9;

    sendBuffer[0] = PACK_HEAD_HIGH;
    sendBuffer[1] = PACK_HEAD_LOW;
    sendBuffer[2] = cmdSize & 0xFF;
    sendBuffer[3] = (cmdSize >> 8) & 0xFF;
    sendBuffer[4] = id;
    sendBuffer[5] = cmd;

    memcpy(sendBuffer + 6, buffer, size);

    crc8                           = calculateCRC(sendBuffer, sendBufferSize);
    sendBuffer[sendBufferSize - 3] = crc8;
    sendBuffer[sendBufferSize - 2] = PACK_END_HIGH;
    sendBuffer[sendBufferSize - 1] = PACK_END_LOW;
    if (serialPort) {
        serialPort->write(reinterpret_cast<const char *>(sendBuffer), sendBufferSize);
    }
}

bool ChainCommon::addRecord(record_list_t *list, record_info_t info)
{
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (!newNode) {
        return false;
    }
    newNode->data = info;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->count++;
    return true;
}

bool ChainCommon::findRecord(record_list_t *list, uint8_t id, record_info_t *result)
{
    node_t *current  = list->head;
    node_t *previous = NULL;

    while (current) {
        if (current->data.id == id) {
            if (result) {
                *result = current->data;
            }

            if (previous) {
                previous->next = current->next;
            } else {
                list->head = current->next;
            }

            free(current);
            list->count--;
            return true;
        }
        previous = current;
        current  = current->next;
    }
    return false;
}
bool ChainCommon::processBufferData(uint16_t id, uint8_t cmd)
{
    uint16_t startIndex = 0;
    bool packetFound    = false;
    while (startIndex < receiveBufferSize) {
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

                uint8_t packetId  = packetData[4];
                uint8_t packetCmd = packetData[5];

                if (packetId == id && packetCmd == cmd) {
                    memcpy(returnPacket, packetData, packetSize);
                    returnPacketSize = packetSize;
                    packetFound      = true;  // 标记找到符合条件的数据包
                } else if (packetCmd == CHAIN_ENUM_PLEASE &&
                           checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                    enumPleaseCount++;  // 枚举
                } else if (packetCmd == 0xE0 &&
                           checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                    if (recordList.count < ACTIVE_PACKET_RECORD_SIZE) {
                        addRecord(&recordList, (record_info_t){packetId, static_cast<uint16_t>((packetData[7] << 8) |
                                                                                               packetData[6])});
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
void ChainCommon::processIncomingData(void)
{
    if (available() > 0) {    // 检测缓冲区是否有数据
        readBuffer();         // 读取缓冲区数据
        processBufferData();  // 处理缓冲区数据
    }
}

bool ChainCommon::available(void)
{
    return serialPort->available() > 0;
}

void ChainCommon::readBuffer(void)
{
    unsigned long startTime = millis();

    while (millis() - startTime < TIMEOUT_MS) {
        if (available() && receiveBufferSize < RECEIVE_BUFFER_SIZE) {
            receiveBuffer[receiveBufferSize++] = serialPort->read();
            startTime                          = millis();
        }
    }
}

bool ChainCommon::waitForData(uint16_t id, uint8_t cmd, uint32_t timeout)
{
    uint32_t startTime = millis();
    while (millis() - startTime < timeout) {
        if (available()) {                             // 检测缓冲区是否有数据
            readBuffer();                              // 读取缓冲区数据
            bool status = processBufferData(id, cmd);  // 处理缓冲区数据
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
    processIncomingData();
    uint16_t temp   = enumPleaseCount;
    enumPleaseCount = 0;
    return temp;
}

chain_status_t ChainCommon::setRGBValue(uint16_t id, rgb_color_t rgb, uint8_t *operationStatus, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = rgb.R;
        cmdBuffer[cmdBufferSize++] = rgb.G;
        cmdBuffer[cmdBufferSize++] = rgb.B;
        sendPacket(id, CHAIN_SET_RGB_VALUE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_SET_RGB_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *operationStatus = returnPacket[6];
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
chain_status_t ChainCommon::getRGBValue(uint16_t id, rgb_color_t *rgb, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_RGB_VALUE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_GET_RGB_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                rgb->R = returnPacket[6];
                rgb->G = returnPacket[7];
                rgb->B = returnPacket[6];
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
    chain_status_t status = CHAIN_OK;
    if (rgbBrightness > 100) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = rgbBrightness;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_SET_RGB_LIGHT, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_SET_RGB_LIGHT, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *operationStatus = returnPacket[6];
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
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_RGB_LIGHT, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_GET_RGB_LIGHT, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *rgbBrightness = returnPacket[6];
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
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *bootloaderVersion = returnPacket[6];
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
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_VERSION_DEVICE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_GET_VERSION_DEVICE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *firmwareVersion = returnPacket[6];
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
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_DEVICE_TYPE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_GET_DEVICE_TYPE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *deviceType = (returnPacket[7] << 8) | returnPacket[6];
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
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = 0x00;
        sendPacket(0xFF, CHAIN_ENUM, cmdBuffer, cmdBufferSize);
        if (waitForData(0xFF, CHAIN_ENUM, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *deviceNum = returnPacket[6];
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

bool ChainCommon::isDeviceConnected(uint8_t maxRetries, unsigned long timeout)
{
    bool status    = true;
    int retryCount = 0;
    if (acquireMutex()) {
        while (retryCount < maxRetries) {
            memset(cmdBuffer, 0, cmdBufferSize);
            cmdBufferSize = 0;
            sendPacket(0xFF, CHAIN_HEARTBEAT, cmdBuffer, cmdBufferSize);
            if (waitForData(0xFF, CHAIN_HEARTBEAT, timeout)) {
                if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                    break;
                }
            }
            retryCount++;
        }
        if (retryCount == maxRetries) {
            status = false;
        }
        releaseMutex();
    } else {
        status = false;
    }
    return status;
}

bool ChainCommon::getDeviceList(device_list_t *list, unsigned long timeout)
{
    chain_status_t status   = CHAIN_OK;
    unsigned long startTime = millis();
    for (uint16_t i = 0; i < list->count; i++) {
        list->devices[i].id = i + 1;
        status              = getDeviceType(i + 1, &list->devices[i].device_type, timeout);
        if (status != CHAIN_OK) {
            return false;
        }
    }
    return true;
}

void ChainCommon::debugPrint(void)
{
    Serial.printf("record :%d\r\n", recordList.count);
}