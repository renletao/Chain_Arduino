#include "M5Unified.h"
#include "Chain.h"

#define TXD_PIN GPIO_NUM_17 // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_16 // 串口接收端口(Rx)
void setup() {
  M5.begin();
  Serial.begin(115200);
  
}


uint8_t update_nums = 0;
void loop() {
  M5.update();
  delay(30);
}
