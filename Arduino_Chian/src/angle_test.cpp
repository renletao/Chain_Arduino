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

//   for (uint8_t i = 0; i < devices_list->count; i++) {
//     if (devices_list->devices[i].device_type == 0x02) {
//       chain.setAngleClockwise(devices_list->devices[i].id, 1, &sta, 1);
//       chain.setRGBLight(devices_list->devices[i].id, 40, &sta, 1);
//       chain.setRGBValue(devices_list->devices[i].id, {0xFF, 0x00, 0xFF},
//       &sta);
//     }
//   }
//   delay(100);
// }

// void loop() {
//   M5.update();
//   for (uint8_t i = 0; i < devices_list->count; i++) {
//     if (devices_list->devices[i].device_type == 0x02) {
//       uint16_t adc16 = 0;
//       uint8_t adc8 = 0;
//       uint8_t ope_sta = 0;
//       uint8_t dir = 0;
//       uint8_t light = 0;
//       chain.getAngle12Adc(devices_list->devices[i].id, &adc16);
//       chain.getAngle8Adc(devices_list->devices[i].id, &adc8);
//       chain.getAngleClockwise(devices_list->devices[i].id, &dir);
//       chain.getRGBLight(devices_list->devices[i].id, &light);
//       Serial.printf("angle 12 value %d\r\n", adc16);
//       Serial.printf("angle 8  value %d\r\n", adc8);
//       Serial.printf("     direction %d\r\n", dir);
//       Serial.printf("         light %d\r\n", light);
//     }
//   }
//   delay(1000);
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