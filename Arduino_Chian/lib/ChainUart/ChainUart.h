/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_UART_H
#define CHAIN_UART_H

#include <ChainCommon.h>

#define CHAIN_UART_TYPE_CODE (0x0006)
typedef enum {
    CHAIN_I2C_INIT                  = 0x10,  // Initialize I2C communication
    CHAIN_I2C_READ                  = 0x11,  // Read data from I2C device
    CHAIN_I2C_WRITE                 = 0x12,  // Write data to I2C device
    CHAIN_I2C_MEM_READ              = 0x13,  // Read data from specific address on I2C device
    CHAIN_I2C_MEM_WRITE             = 0x14,  // Write data to specific address on I2C device
    CHAIN_I2C_GET_DEVICE_ADDR       = 0x15,  // Retrieve address of connected I2C device
    CHAIN_GPIO_OUTPUT_INIT          = 0x30,  // Initialize GPIO for output
    CHAIN_GPIO_INPUT_INIT           = 0x40,  // Initialize GPIO for input
    CHAIN_GPIO_READ_GPIO_LEVEL      = 0x41,  // Read the level of input GPIO pin
    CHAIN_GPIO_EXTERNAL_NVIC_INIT   = 0x50,  // Initialize external interrupt for GPIO
    CHAIN_GPIO_EXTERNAL_NVIC_RETURN = 0x51,  // Return from external interrupt for GPIO
    CHAIN_GPIO_ADC_INIT             = 0x60,  // Initialize ADC for GPIO
    CHAIN_GPIO_ADC_READ             = 0x61,  // Read data from GPIO ADC
    CHAIN_GET_WORK_STATION          = 0x70,  // Query the working status of the system
} uart_cmd_t;                                // Command type for UART communication

#define I2C_READ_MAX_SIZE  (128)  // I2C读取的最大长度
#define I2C_WRITE_MAX_SIZE (128)  // I2C读取的最大长度
#define I2C_ADDR_MAX       (127)  // 7位I2C有效的最大地址

typedef enum {
    I2C_MEMADD_SIZE_8BIT  = 1,  // 8位
    I2C_MEMADD_SIZE_16BIT = 2,  // 16位
} i2c_reg_len_t;
typedef enum {
    CHAIN_I2C_LOW_SPEED_100KHZ = 0x00,  // Low speed (100 kHz)
    CHAIN_I2C_MID_SPEED_400KHZ = 0x01,  // Medium speed (400 kHz)
} i2c_speed_t;                          // Enumeration for I2C speed settings

typedef enum {
    CHAIN_GPIO_PIN_11 = 0x01,  // GPIO pin 11
    CHAIN_GPIO_PIN_12 = 0x02   // GPIO pin 12
} gpio_pin_t;                  // Enumeration for GPIO pin identifiers

typedef enum {
    CHAIN_GPIO_RESET = 0x00,  // Low level (reset)
    CHAIN_GPIO_SET   = 0x01   // High level (set)
} gpio_level_t;               // Enumeration for GPIO pin levels

typedef enum {
    CHAIN_GPIO_OUTPUT_PUSHPULL  = 0x00,  // Push-pull output mode
    CHAIN_GPIO_OUTPUT_OPENDRAIN = 0x01   // Open-drain output mode
} gpio_output_t;                         // Enumeration for GPIO output modes

typedef enum {
    CHAIN_GPIO_PULL_UP   = 0x00,  // Pull-up resistor enabled
    CHAIN_GPIO_PULL_DOWN = 0x01,  // Pull-down resistor enabled
    CHAIN_GPIO_PULL_NO   = 0x02   // No pull-up or pull-down resistor
} gpio_pull_t;                    // Enumeration for external resistor configuration

typedef enum {
    CHAIN_GPIO_SPEED_FREQ_LOW       = 0x00,  // Low speed setting
    CHAIN_GPIO_SPEED_FREQ_MEDIUM    = 0x01,  // Medium speed setting
    CHAIN_GPIO_SPEED_FREQ_HIGH      = 0x02,  // High speed setting
    CHAIN_GPIO_SPEED_FREQ_VERY_HIGH = 0x03   // Very high speed setting
} gpio_speed_t;                              // Enumeration for GPIO speed settings

typedef enum {
    CHAIN_GPIO_MODE_IT_RISING         = 0x00,  // Interrupt on rising edge
    CHAIN_GPIO_MODE_IT_FALLING        = 0x01,  // Interrupt on falling edge
    CHAIN_GPIO_MODE_IT_RISING_FALLING = 0x02   // Interrupt on both rising and falling edges
} nvic_trigger_t;                              // Enumeration for external interrupt trigger sources

typedef enum {
    CHAIN_ADC_CHANNEL_DISABLE = 0x00,  // Disable the ADC channel (set to 0)
    CHAIN_ADC_CHANNEL_ENABLE  = 0x01,  // Enable the ADC channel (set to 1)
} adc_mode_t;                          // Type definition for ADC channel states

typedef enum {
    CHAIN_NOT_WORK_STATUS    = 0x00,  // Not configured working status
    CHAIN_OUTPUT_WORK_STATUS = 0x01,  // Output status
    CHAIN_INPUT_WORK_STATUS  = 0x02,  // Input status
    CHAIN_NVIC_WORK_STATUS   = 0x03,  // External interrupt working status
    CHAIN_ADC_WORK_STATUS    = 0x04,  // ADC working status
    CHAIN_I2C_WORK_STATUS    = 0x05   // I2C working status
} work_status_t;                      // GPIO pin working mode
class ChainUart : virtual public ChainCommon {
public:
    chain_status_t chianUartI2cInit(uint8_t id, uint8_t i2cSpeed, uint8_t *operationStatus,
                                    unsigned long timeout = 100);
    chain_status_t chainUartI2cRead(uint8_t id, uint8_t i2cAddr, uint8_t readLength, uint8_t *buffer,
                                    uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t chainUartI2cWrite(uint8_t id, uint8_t i2cAddr, uint8_t writeLength, uint8_t *buffer,
                                     uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t chainUartI2cMemRead(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, uint8_t regLength,
                                       uint8_t readLength, uint8_t *buffer, uint8_t *operationStatus,
                                       unsigned long timeout = 100);
    chain_status_t chainUartI2cMemWrite(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, uint8_t regLength,
                                        uint8_t writeLength, uint8_t *buffer, uint8_t *operationStatus,
                                        unsigned long timeout = 100);
    chain_status_t getChainUartI2cDeviceAddr(uint8_t id, uint8_t *i2cAddrNums, uint8_t *buffer,
                                             uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t chainUartOutputInit(uint8_t id, uint8_t gpio, uint8_t gpioLevel, uint8_t gpioOutput,
                                       uint8_t gpioPull, uint8_t gpioSpeed, uint8_t *operationStatus,
                                       unsigned long timeout = 100);
    chain_status_t chainUartInputInit(uint8_t id, uint8_t gpio, uint8_t gpioPull, uint8_t *operationStatus,
                                      unsigned long timeout = 100);
    chain_status_t chainUartReadLevel(uint8_t id, uint8_t gpio, uint8_t *gpioLevel, uint8_t *operationStatus,
                                      unsigned long timeout = 100);
    chain_status_t chainUartNvicInit(uint8_t id, uint8_t gpio, uint8_t gpioPull, uint8_t triggerMode,
                                     uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t chainUartAdcInit(uint8_t id, uint8_t adcChannel1, uint8_t adcChannel2, uint8_t *operationStatus,
                                    unsigned long timeout = 100);
    chain_status_t getAdcValue(uint8_t id, uint8_t *adcChannelNums, uint8_t *buffer, uint8_t *operationStatus,
                               unsigned long timeout = 100);
    chain_status_t getUartStatus(uint8_t id, uint8_t *gpio11WorkStatus, uint8_t *gpio12WorkStatus,
                                 unsigned long timeout = 100);
    uint16_t getUartTypeCode(void);

private:
};

#endif  // CHAIN_UART_H