#include "Chain.h"
#include "M5Unified.h"

ChainBase chain;

#define TXD_PIN GPIO_NUM_17 // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_16 // 串口接收端口(Rx)
void setup() {
  M5.begin();
  Serial.begin(115200);
  chain.begin(&Serial2, RXD_PIN, TXD_PIN);
}

uint8_t update_nums = 0;
void loop() {
  M5.update();
  uint16_t keyBuf[KEY_BUFFER_SIZE] = {0};
  size_t keyBufSize = 0;
  chain.processIncomingPacket();
  // uint8_t boot = 0;
  // uint8_t status = chain.getBootloaderVersion(2, &boot);
  // Serial.printf("status %d\r\n", status);
  // Serial.printf("boot %d\r\n", boot);
  // const uint8_t testBuffer[5] = {0x01,0x02,0x03,0x04,0x05};
  // chain.send(reinterpret_cast<const char *>(testBuffer),5);
  chain.getKeyBuffer(keyBuf, &keyBufSize);
  if (keyBufSize > 0) {
    for (uint8_t i = 0; i < keyBufSize; i++) {
      Serial.printf("KEY——PRESS %d的 \r\n", keyBuf[i]);
    }
  }
  delay(100);
}
