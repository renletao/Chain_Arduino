/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_TOF_HPP_
#define _CHAIN_TOF_HPP_

#include <ChainCommon.hpp>

/**
 * @brief Macro for the minimum measurement time of the Chain_ToF sensor.
 */
#define TOF_MEASUREMENT_TIME_MIN (20)

/**
 * @brief Macro for the maximum measurement time of the Chain_ToF sensor.
 */
#define TOF_MEASUREMENT_TIME_MAX (200)

/**
 * @brief Enumeration for Chain_ToF sensor commands.
 *
 * This enumeration defines command codes for various operations of the Chain_ToF sensor.
 */
typedef enum {
    CHAIN_TOF_GET_DISTANCE         = 0x50, /**< Command to get the distance measurement */
    CHAIN_TOF_SET_MEASUREMENT_TIME = 0x51, /**< Command to set the measurement time of Chain_ToF */
    CHAIN_TOF_GET_MEASUREMENT_TIME = 0x52, /**< Command to get the measurement time of the Chain_ToF */
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
    chain_status_t getToFDistance(uint8_t id, uint16_t *distance, unsigned long timeout = 100);

    /**
     * @brief Sets the measurement timing of the ToF device.
     *
     * This function sets the measurement timing (in milliseconds) for the ToF device at the specified position in the
     * chain. The valid range of measurement time is typically from 20ms to 200ms. A longer measurement time increases
     * accuracy but reduces speed. The optional `saveToFlash` parameter can store the setting to the device's flash
     * memory to retain the configuration after a device reboot.
     *
     * @param id The position of the ToF device in the chain (starting from 1).
     * @param time The measurement timing to be set, in milliseconds (e.g., 20–200).
     * @param operationStatus Pointer to store the operation status, 1 for success, 0 for failure.
     * @param timeout Timeout duration in milliseconds (default: 100ms).
     *
     * @return Returns the operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t setToFMeasurementTime(uint8_t id, uint8_t time, uint8_t *operationStatus,
                                         unsigned long timeout = 100);

    /**
     * @brief Gets the current measurement timing of the ToF device.
     *
     * This function retrieves the current measurement timing (in milliseconds) of the ToF device at the specified
     * position in the chain. This timing value reflects how long the device integrates signals before returning a
     * distance result.
     *
     * @param id The position of the ToF device in the chain (starting from 1).
     * @param time Pointer to store the retrieved measurement time, in milliseconds.
     * @param timeout Timeout duration in milliseconds (default: 100ms).
     *
     * @return Returns the operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getToFMeasurementTime(uint8_t id, uint8_t *time, unsigned long timeout = 100);

private:
};

#endif  // _CHAIN_TOF_HPP_
