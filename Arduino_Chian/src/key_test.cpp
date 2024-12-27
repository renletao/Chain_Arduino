// #include "Chain.h"
// #include "M5Unified.h"

// Chain chain;

// device_list_t *devices_list = NULL;

// #define TXD_PIN GPIO_NUM_17  // 串口发送端口(Tx)
// #define RXD_PIN GPIO_NUM_16  // 串口接收端口(Rx)

// bool operation_status;
// // 设备数量
// uint16_t device_nums = 0;
// uint8_t sta          = 0;

// void printDeviceList(device_list_t *devices);
// void setup()
// {
//     M5.begin();
//     Serial.begin(115200);
//     M5.Display.setTextFont(&fonts::efontCN_16);
//     M5.Display.fillScreen(BLACK);
//     M5.Display.setTextScroll(true);
//     chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);
//     if (chain.isDeviceConnected()) {
//         operation_status = chain.getDeviceNum(&device_nums);
//         if (operation_status == CHAIN_OK) {
//             devices_list          = (device_list_t *)malloc(sizeof(device_list_t));
//             devices_list->count   = device_nums;
//             devices_list->devices = (device_info_t *)malloc(sizeof(device_info_t) * device_nums);  // 分配内存
//             operation_status      = chain.getDeviceList(devices_list);
//             Serial.print(operation_status);
//             if (operation_status) {
//                 printDeviceList(devices_list);
//             }
//         } else {
//             Serial.printf("error status:%d \r\n", operation_status);
//             Serial.printf("数据读取失败。。。。\r\n");
//         }
//     }
//     for (uint8_t i = 0; i < devices_list->count; i++) {
//         if (devices_list->devices[i].device_type == chain.getKeyTypeCode()) {
//             chain.setRGBLight(devices_list->devices[i].id, 40, &sta, 0);
//             chain.setRGBValue(devices_list->devices[i].id, {0xFF, 0x00, 0xFF}, &sta);
//         }
//     }
//     delay(5);
// }

// void loop()
// {
//     M5.update();
//     chain_status_t sta;
//     for (uint8_t i = 0; i < devices_list->count; i++) {
//         if (devices_list->devices[i].device_type == chain.getKeyTypeCode()) {
//             uint8_t keyStatus  = 0;
//             uint8_t brightness = 0;
//             keyStatus          = chain.getKeyButtonPressStatus(devices_list->devices[i].id);
//             sta                = chain.getRGBLight(devices_list->devices[i].id, &brightness);
//             if (sta == CHAIN_OK) {
//                 M5.Display.printf(">>> %d\r\n", brightness);
//             }
//             if (keyStatus) {
//                 M5.Display.printf(">>>KEY PRESS\r\n");
//             }
//         }
//     }

//     uint8_t enum_please = chain.getEnumPleaseNum();
//     if (enum_please) {
//         M5.Display.printf("ENUMS PLEASE :%d\r\n", enum_please);
//     }

//     delay(10);
// }

// void printDeviceList(device_list_t *devices)
// {
//     if (devices == NULL) {
//         Serial.println("设备列表为空。");
//         return;
//     }

//     Serial.print("设备数量: ");
//     Serial.println(devices->count);
//     M5.Display.print("  >>> 设备数量: ");
//     M5.Display.println(devices->count);

//     for (uint8_t i = 0; i < devices->count; i++) {
//         Serial.print("设备 ID: ");
//         Serial.println(devices->devices[i].id);
//         Serial.print("设备类型: ");
//         Serial.println(devices->devices[i].device_type);
//         M5.Display.print("  >>> 设备 ID: ");
//         M5.Display.println(devices->devices[i].id);
//         M5.Display.print("  >>> 设备类型: ");
//         M5.Display.println(devices->devices[i].device_type);
//     }
// }