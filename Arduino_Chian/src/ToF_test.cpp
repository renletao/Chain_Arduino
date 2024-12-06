#include "Chain.h"
#include "M5Unified.h"

Chain chain;

device_list_t *devices_list;

#define TXD_PIN GPIO_NUM_21  // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_22  // 串口接收端口(Rx)

chain_status_t operation_status;
// 按键
uint16_t keyBuf[KEY_BUFFER_SIZE] = {0};
size_t keyBufSize                = 0;
// 设备数量
uint16_t device_nums = 0;
uint8_t sta          = 0;

void printDeviceList(device_list_t *devices);
void setup()
{
    M5.begin();
    Serial.begin(115200);
    M5.Display.setTextFont(&fonts::efontCN_24);
    M5.Display.fillScreen(BLACK);
    // M5.Display.setTextScroll(true);
    chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);

    operation_status = chain.getDeviceNum(&device_nums);
    if (operation_status == CHAIN_OK) {
        // 申请内存空间
        devices_list          = (device_list_t *)malloc(sizeof(device_list_t));
        devices_list->count   = device_nums;
        devices_list->devices = (device_t *)malloc(sizeof(device_t) * device_nums);  // 分配内存
        operation_status      = chain.getDeviceList(devices_list);
        Serial.print(operation_status);
        if (operation_status == CHAIN_OK) {
            printDeviceList(devices_list);
        }
    }

    for (uint8_t i = 0; i < devices_list->count; i++) {
        if (devices_list->devices[i].device_type == 0x05) {
            // chain.setAngleClockwise(devices_list->devices[i].id, 1, &sta, 1);
            chain.setRGBLight(devices_list->devices[i].id, 40, &sta, 1);
            chain.setRGBValue(devices_list->devices[i].id, {0xFF, 0x00, 0xFF}, &sta);
            M5.Display.setCursor(0, 90);
            M5.Display.printf("      distance:");
            M5.Display.setCursor(0, 120);
            M5.Display.printf("          mode:");
            M5.Display.setCursor(0, 150);
            M5.Display.printf("         light:");
        }
    }
    delay(100);
}

void loop()
{
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
        for (uint8_t i = 0; i < devices_list->count; i++) {
            if (devices_list->devices[i].device_type == 0x05) {
                M5.Speaker.tone(990, 200);
                chain.setTofMode(devices_list->devices[i].id, 0, &sta, 1);
                if (sta) {
                    Serial.printf(" 短距离模式\r\n");
                }
            }
        }
    }
    if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
        for (uint8_t i = 0; i < devices_list->count; i++) {
            if (devices_list->devices[i].device_type == 0x05) {
                M5.Speaker.tone(990, 200);
                chain.setTofMode(devices_list->devices[i].id, 1, &sta, 1);
                if (sta) {
                    Serial.printf(" 中距离模式\r\n");
                }
            }
        }
    }
    if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(500)) {
        for (uint8_t i = 0; i < devices_list->count; i++) {
            if (devices_list->devices[i].device_type == 0x05) {
                M5.Speaker.tone(990, 200);
                chain.setTofMode(devices_list->devices[i].id, 2, &sta, 1);
                if (sta) {
                    Serial.printf(" 长距离模式\r\n");
                }
            }
        }
    }
    for (uint8_t i = 0; i < devices_list->count; i++) {
        if (devices_list->devices[i].device_type == 0x05) {
            uint16_t distance = 0;
            uint8_t mode      = 0;
            uint8_t ope_sta   = 0;
            uint8_t dir       = 0;
            uint8_t light     = 0;
            chain.getTofMode(devices_list->devices[i].id, &mode);
            chain.getTofDistance(devices_list->devices[i].id, &distance);
            chain.getRGBLight(devices_list->devices[i].id, &light);
            // Serial.printf("      distance: %d\r\n", distance);
            // Serial.printf("          mode: %d\r\n", mode);
            // Serial.printf("         light: %d\r\n", light);
            M5.Display.setCursor(0, 90);
            M5.Display.fillRect(180, 90, 200, 100, BLACK);
            M5.Display.setCursor(180, 90);
            M5.Display.printf(" %d", distance);
            M5.Display.setCursor(180, 120);
            M5.Display.printf(" %d\r\n", mode);
            M5.Display.setCursor(180, 150);
            M5.Display.printf(" %d\r\n", light);
        }
    }
    delay(100);
}

void printDeviceList(device_list_t *devices)
{
    if (devices == NULL) {
        Serial.println("设备列表为空。");
        return;
    }

    Serial.print("设备数量: ");
    Serial.println(devices->count);
    M5.Display.print("  >>> 设备数量: ");
    M5.Display.println(devices->count);

    for (uint8_t i = 0; i < devices->count; i++) {
        Serial.print("设备 ID: ");
        Serial.println(devices->devices[i].id);
        Serial.print("设备类型: ");
        Serial.println(devices->devices[i].device_type);
        M5.Display.print("  >>> 设备 ID: ");
        M5.Display.println(devices->devices[i].id);
        M5.Display.print("  >>> 设备类型: ");
        M5.Display.println(devices->devices[i].device_type);
    }
}
