/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_H
#define CHAIN_H

#include <Arduino.h>

#define PACK_HEAD_HIGH (0xAA) // Packet header
#define PACK_HEAD_LOW (0x55)  // Packet header
#define PACK_END_HIGH (0x55)  // Packet tail
#define PACK_END_LOW (0xAA)   // Packet tail

#define BUFFER_SIZE (1024) // Buffer size

typedef enum {
  CHAIN_OK = 0x00U,            // 操作成功
  CHAIN_PAACKET_ERROR = 0x01U, // 数据包错误
  CHAIN_BUSY = 0x02U,          // 设备繁忙
  CHAIN_TIMEOUT = 0x03U        // 操作超时
} chain_status_t;

typedef enum {
  PACKET_OK = 0,       // 数据包正确
  PACKET_HEADER_ERROR, // 包头错误
  PACKET_TAIL_ERROR,   // 包尾错误
  PACKET_LENGTH_ERROR, // 长度错误
  PACKET_CRC_ERROR     // CRC 错误
} packet_status_t;

typedef enum {
  CHAIN_SET_RGB_VALUE = 0x20, // Command to set RGB values
  CHAIN_GET_RGB_VALUE = 0x21, // Command to get RGB values
  CHAIN_SET_RGB_LIGHT = 0x22, // Command to set the brightness of the RGB light
  CHAIN_GET_RGB_LIGHT = 0x23, // Command to get the brightness of the RGB light
} chain_rgb_cmd_t;            // chain_rgb_cmd_t defines various command types

typedef enum {
  CHAIN_GET_BOOTLOADER_VERSION = 0xF9, // Query bootloader version number
  CHAIN_GET_VERSION_DEVICE = 0xFA,     // Query device software version number
  CHAIN_GET_DEVICE_TYPE = 0xFB,        // Query device type
  CHAIN_ENUM_PLEASE = 0xFC,            // Enumeration request
  CHAIN_HEARTBEAT = 0xFD,              // Heartbeat packet
  CHAIN_ENUM = 0xFE,                   // Enumeration
} chain_base_cmd_t;                    // Command base type

typedef struct {
  uint8_t id = 0;           // 设备ID
  uint16_t device_type = 0; // 设备类型
} device;

typedef struct {
  uint8_t count;   // 设备个数
  device *devices; // 设备数组，使用柔性数组成员
} deviceList;

typedef struct {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGB_Color_TypeDef;

// 枚举获取设备个数
const uint8_t enum_buf[10] = {0xAA, 0x55, 0x04, 0x00, 0xFF,
                              0xFE, 0x00, 0xFD, 0x55, 0xAA};
// 心跳包判断设备是否连接
const uint8_t heartbeat_buf[9] = {0xAA, 0x55, 0x04, 0x00, 0xFF,
                                  0xFD, 0xFC, 0x55, 0xAA};

class ChainBase {
public:
  void begin(HardwareSerial *serial, unsigned long baud = 115200,
             uint32_t config = SERIAL_8N1, int8_t rxPin = -1, int8_t txPin = -1,
             bool invert = false, unsigned long timeout_ms = 10000UL,
             uint8_t rxfifo_full_thrhd = 112UL);
  void sendPacket(uint8_t cmd, uint8_t *buffer, uint16_t size, uint8_t id);
  void receivePacket(uint8_t cmd, uint8_t id);
  packet_status_t checkPacket(const uint8_t *buffer, uint16_t size);

  // 判断是否有设备连接
  bool isDeviceConnected(unsigned long timeout = 100);

  // 获取Bootloader版本号
  chain_status_t getBootloaderVersion(uint8_t id, uint8_t *bootloaderVersion,
                                      unsigned long timeout = 100);

  // 获取软件版本号
  chain_status_t getFirmwareVersion(uint8_t id, uint8_t *fireversionVersion,
                                    unsigned long timeout = 100);

  // 获取设备类型
  chain_status_t getDeviceType(uint8_t id, uint16_t *deviceType,
                               unsigned long timeout = 100);

  // 获取设备数量
  chain_status_t getDeviceNum(uint16_t *deviceNum, unsigned long timeout = 100);

  // 设置RGB灯
  chain_status_t setRGBValue(uint8_t id, RGB_Color_TypeDef rgb, uint8_t *status,
                             unsigned long timeout = 100);
  // 获取RGB灯
  chain_status_t getRGBValue(uint8_t id, RGB_Color_TypeDef *rgb,
                             unsigned long timeout = 100);

  // 设置RGB亮度
  chain_status_t setRGBLight(uint8_t id, uint8_t rgb_light, uint8_t *status,
                             unsigned long timeout = 100);

  // 获取RGB亮度
  chain_status_t getRGBLight(uint8_t id, uint8_t *rgb_light,
                             unsigned long timeout = 100);

private:
  HardwareSerial *serialPort = nullptr;
  uint8_t receiveBuffer[BUFFER_SIZE]; // 接收缓冲区
  size_t receiveBufferSize = 0;       // 当前缓冲区大小
  size_t enumPleaseNum = 0;           // 枚举数据包请求的数量
  bool mutexLocked = false;
  // 获取互斥锁
  bool acquireMutex(void);
  // 释放互斥锁
  void releaseMutex(void);
  // 发送数据
  void send(const char *buffer, size_t size);
  // 判断缓冲区是否有数据
  bool available(void);
  // 取出数据
  void receive(void);
  // 处理主动发送过来的数据包，主要按键，以及枚举请求包
  void processIncomingPacket(void);
  // 增加数据包头
  void addPacketHeader(uint8_t *buffer, uint16_t size);
  // 增加数据包尾
  void addPacketTail(uint8_t *buffer, uint16_t size);
  // 增加数据长度
  void addPacketLength(uint8_t *buffer, uint16_t size);
  // 增加CRC
  void addPacketCRC(uint8_t *buffer, uint16_t size);
  // 检测数据包头
  bool checkPacketHeader(const uint8_t *buffer, uint16_t size);
  // 检测数据包尾
  bool checkPacketTail(const uint8_t *buffer, uint16_t size);
  // 检测CRC
  bool checkCRC(const uint8_t *buffer, uint16_t size);
  // 检测数据长度
  bool checkPacketLength(const uint8_t *buffer, uint16_t size);
  //  计算CRC
  uint8_t calculateCRC(const uint8_t *buffer, uint16_t size);
};

#endif // CHAIN_H