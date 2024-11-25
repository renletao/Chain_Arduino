/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_H
#define CHAIN_H

#include <Arduino.h>

#define TIMEOUT_MS (1) // 超时时间，单位为毫秒

#define PACK_HEAD_HIGH (0xAA) // Packet header
#define PACK_HEAD_LOW (0x55)  // Packet header
#define PACK_END_HIGH (0x55)  // Packet tail
#define PACK_END_LOW (0xAA)   // Packet tail

#define RECEIVE_BUFFER_SIZE (1024) // Buffer size
#define SEND_BUFFER_SIZE (256)     // Buffer size
#define CMD_BUFFER_SIZE (256)
#define KEY_BUFFER_SIZE (16)
typedef enum {
  CHAIN_OK = 0x00U,                  // 操作成功
  CHAIN_RETURN_PACKET_ERROR = 0x01U, // 数据包错误
  CHAIN_BUSY = 0x02U,                // 设备繁忙
  CHAIN_TIMEOUT = 0x03U              // 操作超时
} chain_status_t;

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
} rgb_color_typedef;

class ChainBase {
public:
  uint8_t cmdBuffer[CMD_BUFFER_SIZE] = {0};
  uint16_t cmdBufferSize = 0;
  uint8_t cmdReturnBuffer[CMD_BUFFER_SIZE] = {0};
  size_t cmdReturnBufferSize = 0;
  void begin(HardwareSerial *serial, int8_t rxPin = -1, int8_t txPin = -1,
             unsigned long baud = 115200, uint32_t config = SERIAL_8N1,
             bool invert = false, unsigned long timeout_ms = 10000UL,
             uint8_t rxfifo_full_thrhd = 112UL);
  // 发送数据
  void send(const char *buffer, size_t size);
  void sendPacket(uint8_t cmd, uint8_t *buffer, uint16_t size, uint8_t id);
  bool processPacket(uint8_t cmd, uint8_t id);
  bool processIncomingPacket(void);
  bool checkPacket(const uint8_t *buffer, uint16_t size);
  size_t getEnumPleaseNum(void);
  void getKeyBuffer(uint16_t *outBuffer, size_t *length);
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
  chain_status_t setRGBValue(uint8_t id, rgb_color_typedef rgb, uint8_t *status,
                             unsigned long timeout = 100);
  // 获取RGB灯
  chain_status_t getRGBValue(uint8_t id, rgb_color_typedef *rgb,
                             unsigned long timeout = 100);

  // 设置RGB亮度
  chain_status_t setRGBLight(uint8_t id, uint8_t rgb_light, uint8_t *status,
                             unsigned long timeout = 100);

  // 获取RGB亮度
  chain_status_t getRGBLight(uint8_t id, uint8_t *rgb_light,
                             unsigned long timeout = 100);
private:
  HardwareSerial *serialPort = nullptr;
  uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE] = {0}; // 接收缓冲区
  size_t receiveBufferSize = 0;                     // 当前缓冲区大小
  uint8_t sendBuffer[SEND_BUFFER_SIZE] = {0};       // 接收缓冲区
  size_t sendBufferSize = 0;                        // 当前缓冲区大小
  size_t enumPleaseNum = 0; // 枚举数据包请求的数量
  size_t enumPlease = 0;
  uint16_t keyBuffer[KEY_BUFFER_SIZE] = {0};
  size_t keyBufferSize = 0;
  bool mutexLocked = false;
  // 获取互斥锁
  bool acquireMutex(void);
  // 释放互斥锁
  void releaseMutex(void);
  // 判断缓冲区是否有数据
  bool available(void);
  // 取出数据
  void receive(void);
  // 等待数据
  bool waitForData(uint8_t cmd, uint8_t id, uint32_t timeout);
  // 处理缓冲区的数据
  bool processPacketData(uint8_t cmd, uint8_t id);
  // 增加数据包头
  void addPacketHeader(void);
  // 增加数据包尾
  void addPacketTail(void);
  // 增加数据长度
  void addPacketLength(uint16_t length);
  // 增加CRC
  void addPacketCRC(void);
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