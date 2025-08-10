/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_UART_HPP_
#define _CHAIN_UART_HPP_

#include <ChainCommon.hpp>

/**
 * @brief Maximum size for I2C read operations.
 */
#define I2C_READ_MAX_SIZE (128)

/**
 * @brief Maximum size for I2C write operations.
 */
#define I2C_WRITE_MAX_SIZE (128)

/**
 * @brief Command types for Chain_Uart communication.
 */
typedef enum {
    CHAIN_I2C_INIT                  = 0x10, /**< Initialize I2C communication. */
    CHAIN_I2C_READ                  = 0x11, /**< Read data from I2C device. */
    CHAIN_I2C_WRITE                 = 0x12, /**< Write data to I2C device. */
    CHAIN_I2C_MEM_READ              = 0x13, /**< Read data from specific address on I2C device. */
    CHAIN_I2C_MEM_WRITE             = 0x14, /**< Write data to specific address on I2C device. */
    CHAIN_I2C_SCAN_ADDR             = 0x15, /**< Scan the I2C address of the connection. */
    CHAIN_GPIO_OUTPUT_INIT          = 0x30, /**< Initialize GPIO for output. */
    CHAIN_GPIO_INPUT_INIT           = 0x40, /**< Initialize GPIO for input. */
    CHAIN_GPIO_EXTERNAL_NVIC_INIT   = 0x50, /**< Initialize external interrupt for GPIO. */
    CHAIN_GPIO_READ_GPIO_LEVEL      = 0x41, /**< Read the level of input GPIO pin. */
    CHAIN_GPIO_ADC_INIT             = 0x60, /**< Initialize ADC for GPIO. */
    CHAIN_GPIO_ADC_READ             = 0x61, /**< Read data from GPIO ADC. */
    CHAIN_GET_WORK_STATION          = 0x70, /**< Query the working status of the system. */
    CHAIN_GPIO_EXTERNAL_NVIC_RETURN = 0xE0, /**< Return from external interrupt for GPIO. */
} uart_cmd_t;

/**
 * @brief Enumeration for Chain_Uart operation status.
 */
typedef enum {
    CHAIN_UART_OPERATION_FAIL    = 0x00, /**< Operation failed. */
    CHAIN_UART_OPERATION_SUCCESS = 0x01, /**< Operation successful. */
    CHAIN_WORK_MODE_MISMATCHED   = 0x02, /**< Work mode mismatched. */
} chain_uart_operation_t;

/**
 * @brief Enumeration for I2C register address length.
 */
typedef enum {
    I2C_MEMADD_SIZE_8BIT  = 1, /**< 8-bit address length. */
    I2C_MEMADD_SIZE_16BIT = 2, /**< 16-bit address length. */
} i2c_reg_len_t;

/**
 * @brief Enumeration for I2C communication speed.
 */
typedef enum {
    CHAIN_I2C_LOW_SPEED_100KHZ  = 0x00, /**< Low speed (100 kHz) communication. */
    CHAIN_I2C_HIGH_SPEED_400KHZ = 0x01, /**< High speed (400 kHz) communication. */
} i2c_speed_t;

/**
 * @brief Enumeration for GPIO pin identifiers.
 */
typedef enum {
    CHAIN_GPIO_PIN_1 = 0x01,  // GPIO pin 1
    CHAIN_GPIO_PIN_2 = 0x02   // GPIO pin 2
} gpio_pin_t;

/**
 * @brief Enumeration for GPIO pin levels.
 */
typedef enum {
    CHAIN_GPIO_RESET = 0x00,  // Low level (reset)
    CHAIN_GPIO_SET   = 0x01   // High level (set)
} gpio_level_t;

/**
 * @brief Enumeration for GPIO output modes.
 */
typedef enum {
    CHAIN_GPIO_OUTPUT_PUSHPULL  = 0x00,  // Push-pull output mode
    CHAIN_GPIO_OUTPUT_OPENDRAIN = 0x01   // Open-drain output mode
} gpio_output_t;

/**
 * @brief Enumeration for external resistor configuration.
 */
typedef enum {
    CHAIN_GPIO_PULL_UP   = 0x00,  // Pull-up resistor enabled
    CHAIN_GPIO_PULL_DOWN = 0x01,  // Pull-down resistor enabled
    CHAIN_GPIO_PULL_NO   = 0x02   // No pull-up or pull-down resistor
} gpio_pull_t;

/**
 * @brief Enumeration for GPIO speed settings.
 */
typedef enum {
    CHAIN_GPIO_SPEED_FREQ_LOW       = 0x00,  // Low speed setting
    CHAIN_GPIO_SPEED_FREQ_MEDIUM    = 0x01,  // Medium speed setting
    CHAIN_GPIO_SPEED_FREQ_HIGH      = 0x02,  // High speed setting
    CHAIN_GPIO_SPEED_FREQ_VERY_HIGH = 0x03   // Very high speed setting
} gpio_speed_t;

/**
 * @brief Enumeration for external interrupt trigger sources.
 */
typedef enum {
    CHAIN_GPIO_MODE_IT_RISING         = 0x00,  // Interrupt on rising edge
    CHAIN_GPIO_MODE_IT_FALLING        = 0x01,  // Interrupt on falling edge
    CHAIN_GPIO_MODE_IT_RISING_FALLING = 0x02   // Interrupt on both rising and falling edges
} nvic_trigger_t;

/**
 * @brief Enumeration for ADC channel states.
 */
typedef enum {
    CHAIN_ADC_CHANNEL_DISABLE = 0x00,  // Disable the ADC channel (set to 0)
    CHAIN_ADC_CHANNEL_ENABLE  = 0x01,  // Enable the ADC channel (set to 1)
} adc_mode_t;

/**
 * @brief Enumeration for working status of GPIO pin.
 */
typedef enum {
    CHAIN_NOT_WORK_STATUS    = 0x00,  // Not configured working status
    CHAIN_OUTPUT_WORK_STATUS = 0x01,  // Output status
    CHAIN_INPUT_WORK_STATUS  = 0x02,  // Input status
    CHAIN_NVIC_WORK_STATUS   = 0x03,  // External interrupt working status
    CHAIN_ADC_WORK_STATUS    = 0x04,  // ADC working status
    CHAIN_I2C_WORK_STATUS    = 0x05   // I2C working status
} work_status_t;

class ChainUart : virtual public ChainCommon {
public:
    /**
     * @brief Sets the Chain_Uart I2C mode with specified I2C speed.
     *
     * This function configures the specified Chain_Uart device to operate in I2C mode, setting the I2C communication
     * speed. The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cSpeed The I2C speed to be set, selected from `i2c_speed_t` enumeration:
     *                 - `CHAIN_I2C_LOW_SPEED_100KHZ` for 100 kHz
     *                 - `CHAIN_I2C_HIGH_SPEED_400KHZ` for 400 kHz
     * @param operationStatus Pointer to store the result of the operation, indicating the status of the operation:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t setUartI2cMode(uint8_t id, i2c_speed_t i2cSpeed, uint8_t *operationStatus,
                                  unsigned long timeout = 100);

    /**
     * @brief Reads data from an I2C device through Chain_Uart.
     *
     * This function reads a specified amount of data from an I2C device using Chain_Uart. The data is read from the
     * I2C device at the specified address and stored in the `buffer`. The operation status is returned via the
     * `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cAddr I2C device address to read data from.
     * @param readLength Number of bytes to read from the I2C device, must not exceed `I2C_READ_MAX_SIZE`.
     * @param buffer Pointer to the buffer where the read data will be stored.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t uartI2cRead(uint8_t id, uint8_t i2cAddr, uint8_t readLength, uint8_t *buffer,
                               uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Writes data to an I2C device through Chain_Uart.
     *
     * This function writes a specified amount of data to an I2C device using Chain_Uart. The data to be written is
     * provided in the `buffer`, and the operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cAddr I2C device address to write data to.
     * @param writeLength Number of bytes to write to the I2C device. This value must not exceed `I2C_WRITE_MAX_SIZE`
     * (128 bytes).
     * @param buffer Pointer to the buffer containing the data to be written to the I2C device.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t uartI2cWrite(uint8_t id, uint8_t i2cAddr, uint8_t writeLength, uint8_t *buffer,
                                uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Reads data from a specific memory address of an I2C device through Chain_Uart.
     *
     * This function reads data from a specified register address on an I2C device using Chain_Uart. The register
     * address is provided in `regAddr` and its length is specified by `regLength`. The data to be read is returned
     * in the `buffer`, and the number of bytes to read is specified by `readLength`. The maximum value of `readLength`
     * cannot exceed `I2C_READ_MAX_SIZE`. The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cAddr I2C device address to read data from.
     * @param regAddr Register address to start reading from.
     * @param regLength Length of the register address (refer to `i2c_reg_len_t` with values of `I2C_MEMADD_SIZE_8BIT`
     * or `I2C_MEMADD_SIZE_16BIT`).
     * @param readLength Number of bytes to read from the I2C device, must not exceed `I2C_READ_MAX_SIZE`.
     * @param buffer Pointer to the buffer to store the data read from the I2C device.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t uartI2cMemRead(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, i2c_reg_len_t regLength,
                                  uint8_t readLength, uint8_t *buffer, uint8_t *operationStatus,
                                  unsigned long timeout = 100);

    /**
     * @brief Writes data to a specific memory address of an I2C device through Chain_Uart.
     *
     * This function writes data to a specified register address on an I2C device using Chain_Uart. The register
     * address is provided in `regAddr` and its length is specified by `regLength`. The data to be written is
     * provided in the `buffer`, and the number of bytes to write is specified by `writeLength`. The operation
     * status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * The `writeLength` parameter cannot exceed the maximum allowed size defined by `I2C_WRITE_MAX_SIZE` (128 bytes).
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cAddr I2C device address to write data to.
     * @param regAddr Register address to write data to.
     * @param regLength Length of the register address (refer to `i2c_reg_len_t` with values of `I2C_MEMADD_SIZE_8BIT`
     * or `I2C_MEMADD_SIZE_16BIT`).
     * @param writeLength Number of bytes to write to the I2C device. This value must not exceed `I2C_WRITE_MAX_SIZE`
     * (128 bytes).
     * @param buffer Pointer to the buffer containing the data to be written to the I2C device.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t uartI2cMemWrite(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, i2c_reg_len_t regLength,
                                   uint8_t writeLength, uint8_t *buffer, uint8_t *operationStatus,
                                   unsigned long timeout = 100);

    /**
     * @brief Scans for available I2C addresses using Chain_Uart.
     *
     * This function scans for available I2C addresses by sending a request through Chain_Uart. The addresses of
     * the I2C devices connected to the bus are returned in the `buffer`, and the number of addresses found is
     * returned in `i2cAddrNums`. The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param i2cAddrNums Pointer to store the number of I2C addresses found during the scan.
     * @param buffer Pointer to a buffer that will store the list of scanned I2C addresses.
     * @param size Size of the buffer in bytes.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t getUartI2cScanAddr(uint8_t id, uint8_t *i2cAddrNums, uint8_t *buffer, uint8_t size,
                                      uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Initializes a UART GPIO pin for output using Chain_Uart.
     *
     * This function initializes a specified GPIO pin for output mode with the given configuration parameters.
     * The GPIO pin is identified by `gpio`, and the output level is set to `gpioLevel`. The output mode is
     * specified by `gpioOutputMode`, and the pull-up or pull-down resistor is enabled using `gpioPull`. The
     * speed of the GPIO pin is set to `gpioSpeed`. The operation status is returned via the `operationStatus`
     * parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param gpio GPIO pin to initialize for output.
     * @param gpioLevel Output level to set (refer to `gpio_level_t`).
     * @param gpioOutputMode Output mode to set (refer to `gpio_output_t`).
     * @param gpioPull Pull-up or pull-down resistor to enable (refer to `gpio_pull_t`).
     * @param gpioSpeed Speed of the GPIO pin (refer to `gpio_speed_t`).
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t setUartOutputMode(uint8_t id, gpio_pin_t gpio, gpio_level_t gpioLevel, uint8_t *operationStatus,
                                     gpio_output_t gpioOutputMode = CHAIN_GPIO_OUTPUT_PUSHPULL,
                                     gpio_pull_t gpioPull         = CHAIN_GPIO_PULL_NO,
                                     gpio_speed_t gpioSpeed       = CHAIN_GPIO_SPEED_FREQ_MEDIUM,
                                     unsigned long timeout        = 100);

    /**
     * @brief Initializes a UART GPIO pin for input using Chain_Uart.
     *
     * This function initializes a specified GPIO pin for input mode with the given configuration parameters.
     * The GPIO pin is identified by `gpio`, and the pull-up or pull-down resistor is enabled using `gpioPull`.
     * The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param gpio GPIO pin to initialize for input.
     * @param gpioPull Pull-up or pull-down resistor to enable (refer to `gpio_pull_t`).
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t setUartInputMode(uint8_t id, gpio_pin_t gpio, gpio_pull_t gpioPull, uint8_t *operationStatus,
                                    unsigned long timeout = 100);

    /**
     * @brief Reads the input level of a UART GPIO pin using Chain_Uart.
     *
     * This function reads the input level of a specified GPIO pin for input mode with the given configuration
     * parameters. The GPIO pin is identified by `gpio`, and the pull-up or pull-down resistor is enabled using
     * `gpioPull`. The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param gpio GPIO pin to initialize for input.
     * @param gpioPull Pull-up or pull-down resistor to enable (refer to `gpio_pull_t`).
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */
    chain_status_t getUartInputLevel(uint8_t id, gpio_pin_t gpio, uint8_t *gpioLevel, uint8_t *operationStatus,
                                     unsigned long timeout = 100);

    /**
     * @brief Initializes a UART GPIO pin for input using Chain_Uart.
     *
     * This function initializes a specified GPIO pin for input mode with the given configuration parameters.
     * The GPIO pin is identified by `gpio`, and the pull-up or pull-down resistor is enabled using `gpioPull`.
     * The operation status is returned via the `operationStatus` parameter:
     * - `CHAIN_UART_OPERATION_FAIL` for failure
     * - `CHAIN_UART_OPERATION_SUCCESS` for success
     * - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     *
     * @param id Device position in the chain (starting from 1).
     * @param gpio GPIO pin to initialize for input.
     * @param gpioPull Pull-up or pull-down resistor to enable (refer to `gpio_pull_t`).
     * @param triggerMode Trigger mode to enable (refer to `nvic_trigger_t`).
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */

    chain_status_t setUartNvicMode(uint8_t id, gpio_pin_t gpio, gpio_pull_t gpioPull, nvic_trigger_t triggerMode,
                                   uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the UART NVIC trigger status.
     *
     * This function returns the NVIC trigger status of the UART device.
     *
     * @param id Device position in the chain (starting from 1).
     * @param status Pointer to store the NVIC trigger status.
     *
     * @return Returns true if the operation is successful, false otherwise.
     */
    bool getUartNvicTriggerStatus(uint8_t id, uint16_t *status);

    /**
     * @brief Sets the UART ADC mode.
     *
     * This function sets the ADC mode of the UART device.
     *
     * @param id Device position in the chain (starting from 1).
     * @param channel1 ADC channel 1.
     * @param channel2 ADC channel 2.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the status of the operation (success or failure).
     */

    chain_status_t setUartAdcMode(uint8_t id, adc_mode_t channel1, adc_mode_t channel2, uint8_t *operationStatus,
                                  unsigned long timeout = 100);

    /**
     * @brief Gets the UART ADC value.
     *
     * This function returns the ADC value of the UART device.
     *
     * @param id Device position in the chain (starting from 1).
     * @param channelNums Number of ADC channels.
     * @param buffer Pointer to store the ADC values.
     * @param size Size of the buffer.
     * @param operationStatus Pointer to store the result of the operation, indicating the status:
     *                        - `CHAIN_UART_OPERATION_FAIL` for failure
     *                        - `CHAIN_UART_OPERATION_SUCCESS` for success
     *                        - `CHAIN_WORK_MODE_MISMATCHED` for mode mismatch
     * @param timeout Timeout in milliseconds for the operation (default is 100 ms).
     *
     * @return Returns the ADC value of the UART device.
     */
    chain_status_t getUartAdcValue(uint8_t id, uint8_t *channelNums, uint8_t *buffer, uint8_t size,
                                   uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the UART work status.
     *
     * This function returns the work status of the UART device.
     *
     * @param id Device position in the chain (starting from 1).
     * @param gpio1 GPIO pin 1.
     * @param gpio2 GPIO pin 2.
     *
     * @return Returns the UART work status.
     */
    chain_status_t getUartWorkMode(uint8_t id, work_status_t *gpio1, work_status_t *gpio2, unsigned long timeout = 100);

private:
};

#endif  // _CHAIN_UART_HPP_
