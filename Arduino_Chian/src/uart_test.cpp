#include "Chain.h"
#include "M5Unified.h"

#define UNIT_ENCODER (1)
// #define UNIT_STEP16 (1)
// Scoll 控制
#define SCROLL_ADDR (0x40)
#define ENCODER_REG (0x10)
#define BUTTON_REG  (0x20)
#define RGB_LED_REG (0x31)
// step 控制
#define STEP16_ADDR           (0x48)
#define STEP16_VALUE_REG_ADDR (0x00)

Chain chain;

device_list_t *devices_list = NULL;

#define TXD_PIN GPIO_NUM_17  // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_16  // 串口接收端口(Rx)

uint8_t operation_status;

// 设备数量
uint16_t device_nums = 0;
uint8_t sta          = 0;

void printDeviceList(device_list_t *devices);
void printChainOperationStatus(chain_status_t status);
void printWorkStatus(work_status_t gpio11_status, work_status_t gpio12_status);
void setup()
{
    M5.begin();
    Serial.begin(115200);
    M5.Display.setTextFont(&fonts::efontCN_16);
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextScroll(true);
    chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);
    if (chain.isDeviceConnected()) {
        operation_status = chain.getDeviceNum(&device_nums);
        if (operation_status == CHAIN_OK) {
            devices_list          = (device_list_t *)malloc(sizeof(device_list_t));
            devices_list->count   = device_nums;
            devices_list->devices = (device_info_t *)malloc(sizeof(device_info_t) * device_nums);  // 分配内存
            operation_status      = chain.getDeviceList(devices_list);
            Serial.print(operation_status);
            if (operation_status) {
                printDeviceList(devices_list);
            }
        } else {
            Serial.printf("error status:%d \r\n", operation_status);
            Serial.printf("数据读取失败。。。。\r\n");
        }
    }
    for (uint8_t i = 0; i < devices_list->count; i++) {
        if (devices_list->devices[i].device_type == chain.getUartTypeCode()) {
            chain.setRGBLight(devices_list->devices[i].id, 40, &sta, 0);
            chain.setRGBValue(devices_list->devices[i].id, {0xFF, 0x00, 0xFF}, &sta);
            operation_status = chain.setUartI2cMode(devices_list->devices[i].id, CHAIN_I2C_HIGH_SPEED_400KHZ, &sta);
            printChainOperationStatus((chain_status_t)operation_status);
        }
    }

    delay(5);
}

uint8_t i2c_scan_addr[128] = {0};
uint8_t gpio_11            = 0;
uint8_t gpio_12            = 0;
void loop()
{
    M5.update();
    uint8_t sta = 0;
    for (uint8_t i = 0; i < devices_list->count; i++) {
        if (devices_list->devices[i].device_type == chain.getUartTypeCode()) {
            Serial.println("************************************");
            M5.Display.println("************************************");
            uint8_t i2c_addr_num = 0;
            operation_status =
                chain.getUartI2cScanAddr(devices_list->devices[i].id, &i2c_addr_num, i2c_scan_addr, &sta);
            if (operation_status == CHAIN_OK) {
                if (sta == CHAIN_UART_OPERATION_SUCCESS) {
                    M5.Display.printf("  >>> I2C SCAN ADDR NUMS :%d\r\n", i2c_addr_num);
                    M5.Display.printf("  >>> ");
                    Serial.printf("  >>> I2C SCAN ADDR NUMS :%d\r\n", i2c_addr_num);
                    Serial.printf("  >>> ");
                    for (uint8_t i = 0; i < i2c_addr_num; i++) {
                        M5.Display.printf("0x%02X ", i2c_scan_addr[i]);
                        Serial.printf("0x%02X ", i2c_scan_addr[i]);
                    }
                    M5.Display.println();
                    Serial.println();
                } else {
                    if (sta == CHAIN_UART_OPERATION_FAIL) {
                        M5.Display.printf("  >>> OPERATION_FAIL\r\n");
                        Serial.printf("  >>> OPERATION_FAIL\r\n");
                    } else {
                        M5.Display.printf("  >>> MODE_MISMATCHED\r\n");
                        Serial.printf("  >>> MODE_MISMATCHED\r\n");
                    }
                }
            }
            operation_status = chain.getUartWorkStatus(devices_list->devices[i].id, &gpio_11, &gpio_12);
            if (operation_status == CHAIN_OK) {
                printWorkStatus((work_status_t)gpio_11, (work_status_t)gpio_12);
            }
#if UNIT_ENCODER
            // 指定地址读
            int16_t encode_value = 0;
            // operation_status     = chain.uartI2cMemRead(devices_list->devices[i].id, SCROLL_ADDR, ENCODER_REG,
            //                                             I2C_MEMADD_SIZE_8BIT, 2, (uint8_t *)(&encode_value), &sta);
            // if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
            //     Serial.printf("  >>> encoder_value : %d \r\n", encode_value);
            //     M5.Display.printf("  >>> encoder_value : %d \r\n", encode_value);
            // } else {
            //     Serial.printf("  >>> I2C Read Fail\r\n");
            //     M5.Display.printf("  >>> I2C Read Fail\r\n");
            // }
            // // 指定地址写作
            // uint8_t color[3] = {0xFF, 0x04, 0xBC};
            // operation_status = chain.uartI2cMemWrite(devices_list->devices[i].id, SCROLL_ADDR, RGB_LED_REG,
            //                                          I2C_MEMADD_SIZE_8BIT, 3, color, &sta);
            // if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
            //     Serial.printf("  >>> I2C Write Success\r\n");
            //     M5.Display.printf("  >>> I2C Write Success\r\n");
            // } else {
            //     Serial.printf("  >>> I2C Write Fail\r\n");
            //     M5.Display.printf("  >>> I2C Write Fail\r\n");
            // }

            // I2C写
            uint8_t reg_addr = ENCODER_REG;
            operation_status = chain.uartI2cWrite(devices_list->devices[i].id, SCROLL_ADDR, 1, &reg_addr, &sta);
            if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
                operation_status =
                    chain.uartI2cRead(devices_list->devices[i].id, SCROLL_ADDR, 2, (uint8_t *)(&encode_value), &sta);
                if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
                    Serial.printf("  >>> encoder_value : %d \r\n", encode_value);
                    M5.Display.printf("  >>> encoder_value : %d \r\n", encode_value);
                } else {
                    Serial.printf("  >>> I2C Read Fail\r\n");
                    M5.Display.printf("  >>> I2C Read Fail\r\n");
                }
            } else {
                Serial.printf("  >>> I2C Write Fail\r\n");
                M5.Display.printf("  >>> I2C Write Fail\r\n");
            }
            // I2C写
            uint8_t i2c_write[4] = {0x31, 0x00, 0xFF, 0x88};
            operation_status     = chain.uartI2cWrite(devices_list->devices[i].id, SCROLL_ADDR, 4, i2c_write, &sta);
            if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
                Serial.printf("  >>> I2C Write Success\r\n");
                M5.Display.printf("  >>> I2C Write Success\r\n");
            } else {
                Serial.printf("  >>> I2C Write Fail\r\n");
                M5.Display.printf("  >>> I2C Write Fail\r\n");
            }
#elif UNIT_STEP16
            // I2C 指定地址读
            uint8_t step16_value = 0;
            // operation_status     = chain.uartI2cMemRead(devices_list->devices[i].id, STEP16_ADDR,
            // STEP16_VALUE_REG_ADDR,
            //                                             I2C_MEMADD_SIZE_8BIT, 1, (uint8_t *)(&step16_value), &sta);
            // if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
            //     Serial.printf("  >>> step16_value : %d \r\n", step16_value);
            //     M5.Display.printf("  >>> step16_value : %d \r\n", step16_value);
            // } else {
            //     Serial.printf("  >>> I2C Read Fail\r\n");
            //     M5.Display.printf("  >>> I2C Read Fail\r\n");
            // }
            // I2C写、读
            uint8_t reg_addr = STEP16_VALUE_REG_ADDR;
            operation_status = chain.uartI2cWrite(devices_list->devices[i].id, STEP16_ADDR, 1, &reg_addr, &sta);
            if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
                operation_status =
                    chain.uartI2cRead(devices_list->devices[i].id, STEP16_ADDR, 1, (uint8_t *)(&step16_value), &sta);
                if (operation_status == CHAIN_OK && sta == CHAIN_UART_OPERATION_SUCCESS) {
                    Serial.printf("  >>> step16_value : %d \r\n", step16_value);
                    M5.Display.printf("  >>> step16_value : %d \r\n", step16_value);
                } else {
                    Serial.printf("  >>> I2C Read Fail\r\n");
                    M5.Display.printf("  >>> I2C Read Fail\r\n");
                }
            } else {
                Serial.printf("  >>> I2C Write Fail\r\n");
                M5.Display.printf("  >>> I2C Write Fail\r\n");
            }
#else
#endif
        }
    }

    uint8_t enum_please = chain.getEnumPleaseNum();
    if (enum_please) {
        M5.Display.printf("  >>> ENUMS PLEASE :%d\r\n", enum_please);
    }

    //     delay(10);
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

void printChainOperationStatus(chain_status_t status)
{
    switch (status) {
        case CHAIN_OK:
            Serial.println("  >>> Operation successful.");
            M5.Display.println("  >>> Operation successful.");
            break;
        case CHAIN_PARAMETER_ERROR:
            Serial.println("  >>> Parameter error.");
            M5.Display.println("  >>> Parameter error.");
            break;
        case CHAIN_RETURN_PACKET_ERROR:
            Serial.println("  >>> Return packet error.");
            M5.Display.println("  >>> Return packet error.");
            break;
        case CHAIN_BUSY:
            Serial.println("  >>> Device is busy.");
            M5.Display.println("  >>> Device is busy.");
            break;
        case CHAIN_TIMEOUT:
            Serial.println("  >>> Operation timeout.");
            M5.Display.println("  >>> Operation timeout.");
            break;
        default:
            Serial.println("  >>> Unrecognized status code.");
            M5.Display.println("  >>> Unrecognized status code.");
            break;
    }
}

void printWorkStatus(work_status_t gpio11_status, work_status_t gpio12_status)
{
    Serial.print("  >>> GPIO_11: ");
    M5.Display.print("  >>> GPIO_11: ");
    switch (gpio11_status) {
        case CHAIN_NOT_WORK_STATUS:
            Serial.println("Not configured working status");
            M5.Display.println("Not configured working status");
            break;
        case CHAIN_OUTPUT_WORK_STATUS:
            Serial.println("Output status");
            M5.Display.println("Output status");
            break;
        case CHAIN_INPUT_WORK_STATUS:
            Serial.println("Input status");
            M5.Display.println("Input status");
            break;
        case CHAIN_NVIC_WORK_STATUS:
            Serial.println("External interrupt working status");
            M5.Display.println("External interrupt working status");
            break;
        case CHAIN_ADC_WORK_STATUS:
            Serial.println("ADC working status");
            M5.Display.println("ADC working status");
            break;
        case CHAIN_I2C_WORK_STATUS:
            Serial.println("I2C working status");
            M5.Display.println("I2C working status");
            break;
        default:
            Serial.println("Unrecognized work status");
            M5.Display.println("Unrecognized work status");
            break;
    }

    Serial.print("  >>> GPIO_12: ");
    M5.Display.print("  >>> GPIO_12: ");
    switch (gpio12_status) {
        case CHAIN_NOT_WORK_STATUS:
            Serial.println("Not configured working status.");
            M5.Display.println("Not configured working status.");
            break;
        case CHAIN_OUTPUT_WORK_STATUS:
            Serial.println("Output status.");
            M5.Display.println("Output status.");
            break;
        case CHAIN_INPUT_WORK_STATUS:
            Serial.println("Input status.");
            M5.Display.println("Input status.");
            break;
        case CHAIN_NVIC_WORK_STATUS:
            Serial.println("External interrupt working status.");
            M5.Display.println("External interrupt working status.");
            break;
        case CHAIN_ADC_WORK_STATUS:
            Serial.println("ADC working status.");
            M5.Display.println("ADC working status.");
            break;
        case CHAIN_I2C_WORK_STATUS:
            Serial.println("I2C working status.");
            M5.Display.println("I2C working status.");
            break;
        default:
            Serial.println("Unrecognized work status.");
            M5.Display.println("Unrecognized work status.");
            break;
    }
}
