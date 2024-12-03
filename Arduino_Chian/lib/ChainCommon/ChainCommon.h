/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_COMMON_H
#define CHAIN_COMMON_H

#include <Arduino.h>

// Constants
#define TIMEOUT_MS (1)             // 超时时间，单位为毫秒
#define RECEIVE_BUFFER_SIZE (1024) // Buffer size
#define SEND_BUFFER_SIZE (256)     // Buffer size
#define CMD_BUFFER_SIZE (256)
#define KEY_BUFFER_SIZE (16)

// Packet definitions
#define PACK_HEAD_HIGH (0xAA) // Packet header
#define PACK_HEAD_LOW (0x55)  // Packet header
#define PACK_END_HIGH (0x55)  // Packet tail
#define PACK_END_LOW (0xAA)   // Packet tail

// 操作状态
typedef enum {
  CHAIN_OK = 0x00U,                  // 操作成功
  CHAIN_RETURN_PACKET_ERROR = 0x01U, // 数据包错误
  CHAIN_BUSY = 0x02U,                // 设备繁忙
  CHAIN_TIMEOUT = 0x03U              // 操作超时
} chain_status_t;

// RGB控制命令
typedef enum {
  CHAIN_SET_RGB_VALUE = 0x20, // 设置RGB值
  CHAIN_GET_RGB_VALUE = 0x21, // 获取RGB值
  CHAIN_SET_RGB_LIGHT = 0x22, // 设置RGB亮度
  CHAIN_GET_RGB_LIGHT = 0x23  // 获取RGB亮度
} chain_rgb_cmd_t;

// 基本命令
typedef enum {
  CHAIN_GET_BOOTLOADER_VERSION = 0xF9, // 获取Bootloader版本号
  CHAIN_GET_VERSION_DEVICE = 0xFA,     // 获取设备软件版本号
  CHAIN_GET_DEVICE_TYPE = 0xFB,        // 获取设备类型
  CHAIN_ENUM_PLEASE = 0xFC,            // 枚举请求
  CHAIN_HEARTBEAT = 0xFD,              // 心跳包
  CHAIN_ENUM = 0xFE                    // 枚举
} chain_base_cmd_t;

// 数据结构
typedef struct {
  uint16_t id;          // 设备ID
  uint16_t device_type; // 设备类型
} device_t;

typedef struct {
  uint16_t count;    // 设备数量
  device_t *devices; // 设备数组
} device_list_t;

typedef struct {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} rgb_color;

class ChainCommon {
public:
  // 初始化
  void begin(HardwareSerial *serial, int8_t rxPin = -1, int8_t txPin = -1,
             unsigned long baud = 115200, uint32_t config = SERIAL_8N1,
             bool invert = false, unsigned long timeout_ms = 10000UL,
             uint8_t rxfifo_full_thrhd = 112UL);

  // 数据传输
  void send(const char *buffer, size_t size);
  void sendPacket(uint8_t cmd, const uint8_t *buffer, uint16_t size,
                  uint16_t id);
  bool processPacket(uint8_t cmd, uint16_t id);
  void processIncomingPacket();
  bool checkPacket(const uint8_t *buffer, uint16_t size);

  // RGB操作
  chain_status_t setRGBValue(uint16_t id, rgb_color rgb,
                             uint8_t *operationStatus,
                             unsigned long timeout = 100);
  chain_status_t getRGBValue(uint16_t id, rgb_color *rgb,
                             unsigned long timeout = 100);
  chain_status_t setRGBLight(uint16_t id, uint8_t rgbBrightness,
                             uint8_t *operationStatus,
                             unsigned long timeout = 100);
  chain_status_t getRGBLight(uint16_t id, uint8_t *rgbBrightness,
                             unsigned long timeout = 100);

  // 设备查询
  chain_status_t getBootloaderVersion(uint16_t id, uint8_t *bootloaderVersion,
                                      unsigned long timeout = 100);
  chain_status_t getFirmwareVersion(uint16_t id, uint8_t *firmwareVersion,
                                    unsigned long timeout = 100);
  chain_status_t getDeviceType(uint16_t id, uint16_t *deviceType,
                               unsigned long timeout = 100);
  chain_status_t getDeviceNum(uint16_t *deviceNum, unsigned long timeout = 100);

  // 设备状态
  chain_status_t isDeviceConnected(uint8_t maxRetries = 1,
                                   unsigned long timeout = 10);
  chain_status_t getDeviceList(device_list_t *list,
                               unsigned long timeout = 200);

  // 获取缓存数据
  size_t getEnumPleaseNum();
  void getKeyBuffer(uint16_t *outBuffer, size_t *length);

private:
  HardwareSerial *serialPort; // 串口实例
  // 缓冲区
  uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE] = {0};
  size_t receiveBufferSize = 0;
  uint8_t sendBuffer[SEND_BUFFER_SIZE] = {0};
  size_t sendBufferSize = 0;
  uint8_t cmdBuffer[CMD_BUFFER_SIZE] = {0};
  uint16_t cmdBufferSize = 0;
  uint8_t cmdReturnBuffer[CMD_BUFFER_SIZE] = {0};
  size_t cmdReturnBufferSize = 0;

  // 键缓存
  uint16_t keyBuffer[KEY_BUFFER_SIZE] = {0};
  size_t keyBufferSize = 0;

  // 状态
  bool mutexLocked = false;
  size_t enumPleaseCount = 0;

  // 内部函数
  bool acquireMutex();
  void releaseMutex();
  void receive();
  bool available();
  bool waitForData(uint8_t cmd, uint16_t id, uint32_t timeout);
  bool processPacketData(uint8_t cmd = 0, uint16_t id = 0);
  uint8_t calculateCRC(const uint8_t *buffer, uint16_t size);

  // 数据包操作
  void addPacketHeader();
  void addPacketTail();
  void addPacketLength(uint16_t length);
  void addPacketCRC();
  bool checkPacketHeader(const uint8_t *buffer, uint16_t size);
  bool checkPacketTail(const uint8_t *buffer, uint16_t size);
  bool checkPacketLength(const uint8_t *buffer, uint16_t size);
  bool checkCRC(const uint8_t *buffer, uint16_t size);
};

#endif // CHAIN_COMMON_H