/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "Chain.h"

bool ChainCommon::acquireMutex(void) {
  if (mutexLocked == false) {
    mutexLocked = true;
    return true;
  }
  return false;
}

void ChainCommon::releaseMutex(void) { mutexLocked = false; }

void ChainCommon::begin(HardwareSerial *serial, int8_t rxPin, int8_t txPin,
                        unsigned long baud, uint32_t config, bool invert,
                        unsigned long timeout_ms, uint8_t rxfifo_full_thrhd) {
  serialPort = serial;
  serialPort->begin(baud, SERIAL_8N1, rxPin, txPin, invert, timeout_ms,
                    rxfifo_full_thrhd);
}
void ChainCommon::send(const char *buffer, size_t size) {
  if (serialPort) {
    serialPort->write(buffer, size);
  }
}

// 增加数据包头
void ChainCommon::addPacketHeader(void) {
  sendBuffer[0] = PACK_HEAD_HIGH;
  sendBuffer[1] = PACK_HEAD_LOW;
}
// 增加数据包尾
void ChainCommon::addPacketTail(void) {
  sendBuffer[sendBufferSize - 2] = PACK_END_HIGH;
  sendBuffer[sendBufferSize - 1] = PACK_END_LOW;
}
// 增加数据长度
void ChainCommon::addPacketLength(uint16_t length) {
  sendBuffer[2] = (uint8_t)(length & 0xFF);
  sendBuffer[3] = (uint8_t)((length >> 8) & 0xFF);
}
// 增加CRC
void ChainCommon::addPacketCRC(void) {
  uint8_t crc8 = calculateCRC(sendBuffer, sendBufferSize);
  sendBuffer[sendBufferSize - 3] = crc8;
}
// 检测数据包头
bool ChainCommon::checkPacketHeader(const uint8_t *buffer, uint16_t size) {
  return (size >= 2 && buffer[0] == PACK_HEAD_HIGH &&
          buffer[1] == PACK_HEAD_LOW);
}
// 检测数据包尾
bool ChainCommon::checkPacketTail(const uint8_t *buffer, uint16_t size) {
  return (size >= 2 && buffer[size - 1] == PACK_END_LOW &&
          buffer[size - 2] == PACK_END_HIGH);
}
// 检测CRC
bool ChainCommon::checkCRC(const uint8_t *buffer, uint16_t size) {
  uint8_t crc8 = calculateCRC(buffer, size);
  return (crc8 == buffer[size - 3]);
}
// 检测数据长度
bool ChainCommon::checkPacketLength(const uint8_t *buffer, uint16_t size) {
  uint16_t length = (uint16_t)(buffer[3] << 8) | buffer[2];
  return ((length + 6) == size);
}

//  计算CRC
uint8_t ChainCommon::calculateCRC(const uint8_t *buffer, uint16_t size) {
  uint8_t crc8 = 0;
  for (uint8_t i = 4; i < (size - 3); i++) {
    crc8 += buffer[i];
  }
  return crc8;
}
void ChainCommon::sendPacket(uint8_t cmd, uint8_t *buffer, uint16_t size,
                             uint8_t id) {
  memset(sendBuffer, 0, sendBufferSize); // 清除缓冲
  sendBufferSize = size + 9;
  addPacketHeader();
  addPacketLength(size + 3);
  sendBuffer[4] = id;
  sendBuffer[5] = cmd;
  memcpy(sendBuffer + 6, buffer, size);
  addPacketCRC();
  addPacketTail();
  send(reinterpret_cast<const char *>(sendBuffer), sendBufferSize);
}

// 辅助函数：处理包的提取、头尾检查和数据清理
bool ChainCommon::processPacketData(uint8_t cmd, uint8_t id) {
  size_t startIndex = 0;
  bool packetFound = false; // 标记是否找到符合条件的数据包

  while (startIndex < receiveBufferSize) {
    // 找到数据包的开始标志 0xAA 0x55
    if (receiveBuffer[startIndex] == 0xAA &&
        receiveBuffer[startIndex + 1] == 0x55) {
      // 数据包的长度字节
      if (startIndex + 3 >= receiveBufferSize) {
        return packetFound;
      }

      // 获取数据包长度
      uint16_t length =
          receiveBuffer[startIndex + 2] | (receiveBuffer[startIndex + 3] << 8);

      // 确保包尾正确并且缓冲区剩余足够长度
      if (startIndex + 4 + length + 2 <= receiveBufferSize &&
          receiveBuffer[startIndex + 4 + length] == 0x55 &&
          receiveBuffer[startIndex + 4 + length + 1] == 0xAA) {
        // 包头、包尾、长度加数据内容
        size_t packetSize =
            4 + length +
            2; // 包头（2字节） + 长度（2字节） + 数据内容 + 包尾（2字节）

        // 直接处理缓冲区中的数据，不分配新内存
        uint8_t *packetData =
            receiveBuffer + startIndex; // 数据包从 startIndex 开始

        // 更新缓冲区大小
        size_t remainingSize = receiveBufferSize - packetSize;
        receiveBufferSize = remainingSize;
        uint8_t packetId = packetData[4];
        uint8_t packetCmd = packetData[5];
        uint8_t packetData6 = packetData[6];
        if (packetId == id && packetCmd == cmd) {
          memcpy(cmdReturnBuffer, packetData, packetSize);
          cmdReturnBufferSize = packetSize;
          packetFound = true; // 标记找到符合条件的数据包
        } else if (packetCmd == CHAIN_ENUM_PLEASE &&
                   checkCRC(reinterpret_cast<const uint8_t *>(cmdReturnBuffer),
                            cmdReturnBufferSize)) {
          enumPlease++; // 计数器增加
        } else if (packetCmd == 0x40 && packetData6 == 0x11 &&
                   checkCRC(reinterpret_cast<const uint8_t *>(cmdReturnBuffer),
                            cmdReturnBufferSize)) {
          if (keyBufferSize < KEY_BUFFER_SIZE) {
            keyBuffer[keyBufferSize++] = packetId;
          }
        }

        // 如果缓冲区还有未处理的数据，继续处理
        if (remainingSize > 0) {
          // 直接从下一个数据包的开始位置继续处理
          memmove(receiveBuffer, receiveBuffer + startIndex + packetSize,
                  remainingSize);
          startIndex = 0; // 从新的数据开始处理
        } else {
          // 没有剩余数据，处理结束
          break;
        }
      } else {
        break; // 如果包尾不匹配或长度不足，跳出循环
      }
    } else {
      startIndex++; // 如果当前不是包头，继续向后查找
    }
  }

  // 清除缓冲区和大小
  memset(receiveBuffer, 0, sizeof(receiveBuffer)); // 清空缓冲区
  receiveBufferSize = 0;                           // 重置缓冲区大小
  return packetFound;
}
bool ChainCommon::processPacket(uint8_t cmd, uint8_t id) {
  return processPacketData(cmd, id);
}
void ChainCommon::processIncomingPacket(void) {
  if (available() > 0) {
    receive();
    processPacketData();
  }
}
bool ChainCommon::checkPacket(const uint8_t *buffer, uint16_t size) {
  return (checkPacketHeader(buffer, size) && checkPacketTail(buffer, size) &&
          checkCRC(buffer, size) && checkPacketLength(buffer, size));
}

bool ChainCommon::available(void) { return serialPort->available() > 0; }
// 取出数据
void ChainCommon::receive(void) {
  unsigned long startTime = millis();

  while (true) // 无限循环
  {
    // 检查超时
    if (millis() - startTime >= TIMEOUT_MS) {
      break; // 超时后退出循环
    }

    // 检查是否有可用数据
    while (available()) {
      if (receiveBufferSize < sizeof(receiveBuffer)) { // 防止越界
        receiveBuffer[receiveBufferSize++] = serialPort->read();
        startTime = millis();
      } else {
        break;
      }
    }
  }
}

bool ChainCommon::waitForData(uint8_t cmd, uint8_t id, uint32_t timeout) {
  uint32_t startTime = millis(); // 获取当前时间
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

size_t ChainCommon::getEnumPleaseNum(void) {
  size_t temp = enumPlease;
  enumPlease = 0;
  return temp;
}
void ChainCommon::getKeyBuffer(uint16_t *outBuffer, size_t *length) {
  *length = keyBufferSize; // 返回当前keyBuffer的有效数据长度

  // 复制keyBuffer的内容到outBuffer
  memcpy(outBuffer, keyBuffer, keyBufferSize * sizeof(uint16_t));

  // 清空keyBuffer
  memset(keyBuffer, 0, KEY_BUFFER_SIZE); // 清空keyBuffer数组
  keyBufferSize = 0;                     // 重置keyBufferSize
}
chain_status_t ChainCommon::getBootloaderVersion(uint8_t id,
                                                 uint8_t *bootloaderVersion,
                                                 unsigned long timeout) {
  chain_status_t status = CHAIN_OK;
  if (acquireMutex()) {
    memset(cmdBuffer, 0, cmdBufferSize);
    cmdBufferSize = 0;
    sendPacket(CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize, id);

    if (waitForData(CHAIN_GET_BOOTLOADER_VERSION, id, timeout)) {
      if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer),
                      cmdReturnBufferSize)) {
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
