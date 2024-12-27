/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_JOYSTICK_H
#define CHAIN_JOYSTICK_H

#include <ChainCommon.h>

/**
 * @brief CHAIN_JOYSTICK device type code.
 */
#define CHAIN_JOYSTICK_DEVICE_TYPE_CODE (0x0004)

/**
 * @brief CHAIN_JOYSTICK button press status code.
 */
#define CHAIN_JOYSTICK_BUTTON_PRESS_TYPE_CODE (0x0001)

/**
 * @brief Enumeration for Chain_Joystick commands.
 *
 * This enumeration defines command codes for various operations on the Chain_Joystick device.
 * The commands include getting ADC values, setting mapped ranges, and retrieving mapped values for both axes.
 */
typedef enum {
    CHAIN_JOYSTICK_GET_16ADC                     = 0x30, /**< Command to get 16-bit ADC values */
    CHAIN_JOYSTICK_GET_8ADC                      = 0x31, /**< Command to get 8-bit ADC values */
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE       = 0x32, /**< Command to get mapped range for X and Y axes */
    CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE       = 0x33, /**< Command to set mapped range for X and Y axes */
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE = 0x34, /**< Command to get 16-bit mapped values for X and Y */
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE  = 0x35, /**< Command to get 8-bit mapped values for X and Y */
} chain_joystick_cmd_t;
class ChainJoystick : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the ADC values of the joystick.
     *
     * This function reads the ADC values of the joystick for the device at the specified position in the chain.
     * It returns the ADC values for both the X and Y axes.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param xAdcValue Pointer to store the ADC value for the X-axis.
     * @param yAdcValue Pointer to store the ADC value for the Y-axis.
     * @param timeout Maximum time to wait for the ADC values in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getJoystick16Adc(uint8_t id, uint16_t *xAdcValue, uint16_t *yAdcValue, unsigned long timeout = 100);

    /**
     * @brief Retrieves the 8-bit ADC values of the joystick.
     *
     * This function reads the 8-bit ADC values of the joystick for the device at the specified position in the chain.
     * It returns the 8-bit ADC values for both the X and Y axes.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param xAdcValue Pointer to store the 8-bit ADC value for the X-axis.
     * @param yAdcValue Pointer to store the 8-bit ADC value for the Y-axis.
     * @param timeout Maximum time to wait for the ADC values in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getJoystick8Adc(uint8_t id, uint8_t *xAdcValue, uint8_t *yAdcValue, unsigned long timeout = 100);

    /**
     * @brief Retrieves the joystick mapped range values.
     *
     * This function retrieves the mapped range values of the joystick at the specified position,
     * typically mapping the joystick's X and Y ADC values to a specified range.
     * The mapped values are stored in the provided `mapBufValue` buffer.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param mapBufValue The buffer where the retrieved mapped range values will be stored.
     * @param size The size of the buffer, limiting the number of mapped values to be returned.
     * @param timeout The timeout duration for retrieving the data in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t getJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size, unsigned long timeout = 100);

    /**
     * @brief Sets the joystick mapped range values.
     *
     * This function sets the mapped range values for the joystick at the specified position.
     * The mapped values are set based on the data in the provided `mapBufValue` buffer.
     * Optionally, the settings can be saved to flash memory.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param mapBufValue The buffer containing the mapped range values to be set.
     * @param size The size of the buffer, indicating the number of mapped values.
     * @param operationStatus A pointer to store the operation status after completion. Returns 1 if successful, 0 if
     * failed.
     * @param saveToFlash Whether to save the settings to flash memory; 0 for no, 1 for yes (default: 0).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t setJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size, uint8_t *operationStatus,
                                          uint8_t saveToFlash = 0, unsigned long timeout = 100);
    /**
     * @brief Maps the joystick's 16-bit ADC values to final signed int16 range values.
     *
     * This function maps the joystick's X and Y axis 16-bit ADC values to signed int16 range values based on the
     * joystick's internal mapping range. The mapping process converts the ADC values to the appropriate range according
     * to preset intervals, returning the final mapped X and Y values. The returned values are signed int16,
     * representing the mapped results of the joystick's X and Y axes.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param xMapAdcValue A pointer to store the mapped value for the joystick's X-axis (int16 type).
     * @param yMapAdcValue A pointer to store the mapped value for the joystick's Y-axis (int16 type).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t getJoystickMappedInt16Value(uint8_t id, int16_t *xMapAdcValue, int16_t *yMapAdcValue,
                                               unsigned long timeout = 100);
    /**
     * @brief Maps the joystick's 16-bit ADC values to final signed int8 range values.
     *
     * This function maps the joystick's X and Y axis 16-bit ADC values to signed int8 range values based on the
     * joystick's internal mapping range. The mapping process converts the ADC values to the appropriate range according
     * to preset intervals, returning the final mapped X and Y values. The returned values are signed int8, representing
     * the mapped results of the joystick's X and Y axes with a range of -127 to 127.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param xMapAdcValue A pointer to store the mapped value for the joystick's X-axis (int8 type).
     * @param yMapAdcValue A pointer to store the mapped value for the joystick's Y-axis (int8 type).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t getJoystickMappedInt8Value(uint8_t id, int8_t *xMapAdcValue, int8_t *yMapAdcValue,
                                              unsigned long timeout = 100);
    /**
     * @brief Retrieves the Chain_Joystick type code.
     *
     * This function retrieves the type code of the Chain_Joystick device, used to identify the joystick's device type.
     *
     * @return Returns the Chain_Joystick device type code.
     */
    uint16_t getJoystickTypeCode(void);

    /**
     * @brief Retrieves the joystick button press status.
     *
     * This function queries whether the joystick button at the specified position is pressed.
     * The returned value indicates whether the button is in a pressed state.
     *
     * @param id The position of the device in the chain (starting from 1).
     *
     * @return Returns `true` if the joystick button is pressed, `false` if the joystick button is not pressed.
     */
    bool getJoystickButtonPressStatus(uint8_t id);

private:
};

#endif  // CHAIN_JOYSTICK_H