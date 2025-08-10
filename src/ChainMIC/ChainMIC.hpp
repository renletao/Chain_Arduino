/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_MIC_HPP_
#define _CHAIN_MIC_HPP_

#include <ChainCommon.hpp>


typedef enum {
    CHAIN_MIC_GET_12ADC           = 0x30,
    CHAIN_MIC_SET_THRESHOLD_VALUE = 0x31,
    CHAIN_MIC_GET_THRESHOLD_VALUE = 0x32,
    CHAIN_MIC_SET_MODE            = 0xE1,
    CHAIN_MIC_GET_MODE            = 0xE2,
    CHAIN_MIC_SET_TRIGGER_CYCLE   = 0xE3,
    CHAIN_MIC_GET_TRIGGER_CYCLE   = 0xE4,
} chain_mic_cmd_t;

class ChainMIC : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the 12-bit ADC value of the MIC device.
     *
     * This function fetches the 12-bit ADC value from the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param adcValue Pointer to store the retrieved 12-bit ADC value.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMIC12Adc(uint8_t id, uint16_t *adcValue, unsigned long timeout = 100);

    /**
     * @brief Sets the threshold value for the MIC device.
     *
     * This function sets the threshold value for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param thresholdValue The threshold value to set.
     * @param saveToFlash Whether to save the threshold value to flash (default is 0).
     * @param operationStatus Pointer to store the operation status.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICThresholdValue(uint8_t id, uint16_t thresholdValue, uint8_t *operationStatus,uint8_t saveToFlash = 0,
                                        unsigned long timeout = 100);

    /**
     * @brief Gets the threshold value of the MIC device.
     *
     * This function retrieves the threshold value of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param thresholdValue Pointer to store the retrieved threshold value.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICThresholdValue(uint8_t id, uint16_t *thresholdValue, unsigned long timeout = 100);

    /**
     * @brief Sets the mode for the MIC device.
     *
     * This function sets the mode for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param mode The mode to set.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICMode(uint8_t id, uint8_t mode, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the mode of the MIC device.
     *
     * This function retrieves the mode of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param mode Pointer to store the retrieved mode.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICMode(uint8_t id, uint8_t *mode, unsigned long timeout = 100);

    /**
     * @brief Sets the trigger cycle for the MIC device.
     *
     * This function sets the trigger cycle for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param triggerCycle The trigger cycle to set.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICTriggerCycle(uint8_t id, uint16_t triggerCircle, uint8_t *operationStatus,
                                       unsigned long timeout = 100);
    

    /**
     * @brief Gets the trigger cycle of the MIC device.
     *
     * This function retrieves the trigger cycle of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param triggerCycle Pointer to store the retrieved trigger cycle.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICTriggerCycle(uint8_t id, uint16_t *triggerCycle, unsigned long timeout = 100);


    bool getMICTriggerStatus(uint8_t id, uint16_t *triggerStatus);

private:
};

#endif  // _CHAIN_MIC_HPP_
