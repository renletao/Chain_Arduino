// #include "Chain.h"
// #include "M5Unified.h"

// Chain chain;

// device_list_t *devices_list;

// #define TXD_PIN GPIO_NUM_17 // 串口发送端口(Tx)
// #define RXD_PIN GPIO_NUM_16 // 串口接收端口(Rx)

// chain_status_t operation_status;
// // 按键
// uint16_t keyBuf[KEY_BUFFER_SIZE] = {0};
// size_t keyBufSize = 0;
// // 设备数量
// uint16_t device_nums = 0;
// uint8_t sta = 0;

// void printDeviceList(device_list_t *devices);
// void setup() {
//   M5.begin();
//   Serial.begin(115200);
//   M5.Display.setTextFont(&fonts::efontCN_16);
//   M5.Display.fillScreen(BLACK);
//   M5.Display.setTextScroll(true);
//   chain.begin(&Serial2, RXD_PIN, TXD_PIN);

//   operation_status = chain.getDeviceNum(&device_nums);
//   if (operation_status == CHAIN_OK) {
//     // 申请内存空间
//     devices_list = (device_list_t *)malloc(sizeof(device_list_t));
//     devices_list->count = device_nums;
//     devices_list->devices =
//         (device_t *)malloc(sizeof(device_t) * device_nums); // 分配内存
//     operation_status = chain.getDeviceList(devices_list);
//     Serial.print(operation_status);
//     if (operation_status == CHAIN_OK) {
//       printDeviceList(devices_list);
//     }
//   }
//   delay(100);
// }

// uint16_t mapRange[8] = {0};
// void loop() {
//   M5.update();
//   if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
//     M5.Speaker.tone(990, 200);
//     chain_status_t operation_status;
//     uint32_t xSum = 0;
//     uint32_t ySum = 0;
//     uint8_t count = 0;
//     uint16_t xAdcValue = 0;
//     uint16_t yAdcValue = 0;
//     if (devices_list->devices[0].device_type == 0x04) {
//       for (uint8_t i = 0; i < 100; i++) {
//         operation_status =
//         chain.getJoystick16Adc(devices_list->devices[0].id,
//                                                   &xAdcValue, &yAdcValue);
//         if (operation_status == CHAIN_OK) {
//           count++;
//           xSum = xSum + xAdcValue;
//           ySum = ySum + yAdcValue;
//           Serial.printf("Joystick     16 ADC X:%d  Y:%d\r\n", xAdcValue,
//                         yAdcValue);
//         }
//         delay(10);
//       }
//       xAdcValue = xSum / count;
//       yAdcValue = ySum / count;
//       mapRange[1] = xAdcValue - 2000;
//       mapRange[2] = xAdcValue + 2000;
//       mapRange[5] = yAdcValue - 2000;
//       mapRange[6] = yAdcValue + 2000;
//     }
//   }
//   if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
//     M5.Speaker.tone(990, 200);
//     chain_status_t operation_status;
//     if (devices_list->devices[0].device_type == 0x04) {
//       operation_status = chain.setJoystickMappedRange(
//           devices_list->devices[0].id, mapRange, 8, &sta, 1);
//       if (operation_status == CHAIN_OK) {
//         Serial.println("设置OK");
//         delay(5000);
//       }
//     }
//   }
//   if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(500)) {
//     M5.Speaker.tone(990, 200);
//     chain_status_t operation_status;
//     uint16_t xAdcValue = 0;
//     uint16_t yAdcValue = 0;
//     uint16_t xAdcValue_min = 65535;
//     uint16_t xAdcValue_max = 0;
//     uint16_t yAdcValue_min = 65535;
//     uint16_t yAdcValue_max = 0;
//     if (devices_list->devices[0].device_type == 0x04) {
//       while (1) {
//         M5.update();
//         if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
//           M5.Speaker.tone(990, 200);
//           break;
//         }
//         operation_status =
//         chain.getJoystick16Adc(devices_list->devices[0].id,
//                                                   &xAdcValue, &yAdcValue);
//         if (operation_status == CHAIN_OK) {
//           if (xAdcValue > xAdcValue_max) {
//             xAdcValue_max = xAdcValue;
//           }
//           if (xAdcValue < xAdcValue_min) {
//             xAdcValue_min = xAdcValue;
//           }
//           if (yAdcValue > yAdcValue_max) {
//             yAdcValue_max = yAdcValue;
//           }
//           if (yAdcValue < yAdcValue_min) {
//             yAdcValue_min = yAdcValue;
//           }
//           Serial.printf("Joystick     16 ADC X:%d  Y:%d\r\n", xAdcValue,
//                         yAdcValue);
//         }
//         delay(100);
//       }
//       mapRange[0] = xAdcValue_min + 2000;
//       mapRange[3] = xAdcValue_max - 2000;
//       mapRange[4] = yAdcValue_min + 2000;
//       mapRange[7] = yAdcValue_max - 2000;
//     }
//   }

//   Serial.printf("                MAP RANGE\r\n");
//   for (uint8_t i = 0; i < 8; i++) {
//     Serial.printf(" %d ", mapRange[i]);
//   }
//   Serial.println();
//   delay(100);
// }

// void printDeviceList(device_list_t *devices) {
//   if (devices == NULL) {
//     Serial.println("设备列表为空。");
//     return;
//   }

//   Serial.print("设备数量: ");
//   Serial.println(devices->count);
//   M5.Display.print("  >>> 设备数量: ");
//   M5.Display.println(devices->count);

//   for (uint8_t i = 0; i < devices->count; i++) {
//     Serial.print("设备 ID: ");
//     Serial.println(devices->devices[i].id);
//     Serial.print("设备类型: ");
//     Serial.println(devices->devices[i].device_type);
//     M5.Display.print("  >>> 设备 ID: ");
//     M5.Display.println(devices->devices[i].id);
//     M5.Display.print("  >>> 设备类型: ");
//     M5.Display.println(devices->devices[i].device_type);
//   }
// }