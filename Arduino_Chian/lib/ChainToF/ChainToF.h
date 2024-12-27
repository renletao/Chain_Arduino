/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_TOF_H
#define CHAIN_TOF_H

#include <ChainCommon.h>

/**
 * @brief CHAIN_TOF device type code.
 */
#define CHAIN_TOF_DEVICE_TYPE_CODE (0x0005)

/**
 * @brief TOF sensor range modes.
 *
 * This enumeration defines the modes for the TOF (Time of Flight) sensor,
 * specifying the range of distance measurement.
 */
typedef enum {
    TOF_SHORT  = 0x00, /**< Short range mode */
    TOF_MEDIUM = 0x01, /**< Medium range mode */
    TOF_LONG   = 0x02, /**< Long range mode */
} tof_mode_t;

/**
 * @brief Enumeration for Chain_ToF sensor commands.
 *
 * This enumeration defines command codes for various operations of the Chain_ToF sensor.
 */
typedef enum {
    CHAIN_TOF_GET_DISTANCE = 0x50, /**< Command to get the distance measurement */
    CHAIN_TOF_SET_MODE     = 0x51, /**< Command to set the mode of Chain_ToF */
    CHAIN_TOF_GET_MODE     = 0x52, /**< Command to get the current mode of the Chain_ToF */
} chain_tof_cmd_t;

class ChainToF : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the measured distance from the ToF device.
     *
     * This function retrieves the measured distance from the ToF device located at the specified position
     * in the chain. The distance is calculated based on the Time-of-Flight measurement from the device.
     * If the data cannot be obtained within the specified timeout period, the function returns the appropriate error
     * status.
     *
     * @param id The position of the ToF device in the chain (starting from 1).
     * @param distance Pointer to store the retrieved distance value.
     * @param timeout Timeout duration (default: 100ms).
     *
     * @return Returns the operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getTofDistance(uint8_t id, uint16_t *distance, unsigned long timeout = 100);

    /**
     * @brief Sets the measurement mode of the ToF device.
     *
     * This function sets the measurement mode for the ToF device at the specified position in the chain.
     * The supported measurement modes are short-range, mid-range, and long-range modes.
     * After setting the mode, the device will perform measurements according to the selected mode.
     * The optional `saveToFlash` parameter can store the setting to the device's flash memory to retain the
     * configuration after a device reboot.
     *
     * @param id The position of the ToF device in the chain (starting from 1).
     * @param mode The measurement mode to be set, supported values are:
     *              - 0x00: Short-range mode
     *              - 0x01: Mid-range mode
     *              - 0x02: Long-range mode
     * @param operationStatus Pointer to store the operation status, 1 for success, 0 for failure.
     * @param saveToFlash Whether to save the setting to flash (default: 0, do not save).
     * @param timeout Timeout duration in milliseconds (default: 100ms).
     *
     * @return Returns the operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t setTofMode(uint8_t id, uint8_t mode, uint8_t *operationStatus, uint8_t saveToFlash = 0,
                              unsigned long timeout = 100);

    /**
     * @brief Gets the current measurement mode of the ToF device.
     *
     * This function retrieves the current measurement mode of the ToF device at the specified position in the chain.
     * The returned mode value can be short-range, mid-range, or long-range mode.
     * This mode value helps the user understand the current working state of the device.
     *
     * @param id The position of the ToF device in the chain (starting from 1).
     * @param mode Pointer to store the retrieved measurement mode. The returned mode values are:
     *             - 0x00: Short-range mode
     *             - 0x01: Mid-range mode
     *             - 0x02: Long-range mode
     * @param timeout Timeout duration in milliseconds (default: 100ms).
     *
     * @return Returns the operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getTofMode(uint8_t id, uint8_t *mode, unsigned long timeout = 100);

    /**
     * @brief Gets the Chain_ToF device type code.
     *
     * This function returns the type code for the Chain_ToF device, which is used to
     * identify the device type in the system.
     *
     * @return Returns the Chain_ToF device type code.
     */
    uint16_t getToFTypeCode(void);

private:
};

#endif  // CHAIN_TOF_H