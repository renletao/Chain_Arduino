/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_ENCODER_H
#define CHAIN_ENCODER_H

#include <ChainCommon.h>

/**
 * @brief CHAIN_ENCODER device type code.
 */
#define CHAIN_ENCODER_DEVICE_TYPE_CODE (0x0001)

/**
 * @brief CHAIN_ENCODER button press status code.
 */
#define CHAIN_ENCODER_BUTTON_PRESS_TYPE_CODE (0x0001)

/**
 * @brief Enumeration for AB pin status.
 *
 * This enumeration defines the status of the AB pin, indicating the direction of rotation.
 */
typedef enum {
    ENCODER_AB = 0x00,  // AB, increases clockwise
    ENCODER_BA = 0x01,  // BA, decreases clockwise
} ab_t;

/**
 * @brief Command types for the Chain_Encoder.
 *
 * This enumeration defines the command types used for Chain_Encoder operations, such as getting encoder values,
 * resetting values, and setting or getting the AB status of the encoder.
 */
typedef enum {
    CHAIN_ENCODER_GET_VALUE       = 0x10, /**< Get encoder value. */
    CHAIN_ENCODER_GET_INC_VALUE   = 0x11, /**< Get encoder increment value. */
    CHAIN_ENCODER_RESET_VALUE     = 0x13, /**< Reset encoder value. */
    CHAIN_ENCODER_RESET_INC_VALUE = 0x14, /**< Reset encoder increment value. */
    CHAIN_ENCODER_SET_AB_STATUS   = 0x15, /**< Set AB status, 0->AB, 1->BA. */
    CHAIN_ENCODER_GET_AB_STATUS   = 0x16, /**< Get AB status, 0->AB, 1->BA. */
} CHAIN_ENCODER_CMD_T;

class ChainEncoder : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the encoder value.
     *
     * This function retrieves the current encoder value for the specified device in the chain.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param value Pointer to the variable where the encoder value will be stored.
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (e.g., success or failure).
     */
    chain_status_t getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout = 100);

    /**
     * @brief Retrieves the encoder increment value.
     *
     * This function retrieves the increment value of the encoder for the specified device in the chain.
     * The increment value represents the change in encoder position since the last read.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param incValue Pointer to the variable where the increment value will be stored.
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (e.g., success or failure).
     */
    chain_status_t getEncoderIncValue(uint8_t id, int16_t *incValue, unsigned long timeout = 100);

    /**
     * @brief Resets the encoder value.
     *
     * This function resets the encoder value to its initial state for the specified device in the chain.
     * It is typically used when the encoder value needs to be cleared or initialized.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param operationStatus Pointer to the variable where the operation status (1 for success, 0 for failure) will be
     * stored.
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (1 for success, 0 for failure).
     */
    chain_status_t resetEncoderValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Resets the encoder increment value.
     *
     * This function resets the encoder increment value to its initial state for the specified device in the chain.
     * It is typically used when the increment value needs to be cleared or initialized.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param operationStatus Pointer to the variable where the operation status (1 for success, 0 for failure) will be
     * stored.
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (1 for success, 0 for failure).
     */
    chain_status_t resetEncoderIncValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Sets the encoder AB direction.
     *
     * This function sets the AB direction of the encoder for the specified device in the chain.
     * AB means the direction increases clockwise, and BA means the direction increases counterclockwise.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param direct The direction to set: 0 for AB (clockwise increase), 1 for BA (counterclockwise increase).
     * @param operationStatus Pointer to the variable where the operation status (1 for success, 0 for failure) will be
     * stored.
     * @param saveToFlash Optional parameter to save the setting to flash memory (default is 0, not save).
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (1 for success, 0 for failure).
     */
    chain_status_t setEncoderABDirect(uint8_t id, uint8_t direct, uint8_t *operationStatus, uint8_t saveToFlash = 0,
                                      unsigned long timeout = 100);

    /**
     * @brief Gets the encoder AB direction.
     *
     * This function retrieves the current AB direction of the encoder for the specified device in the chain.
     * AB means the direction increases clockwise, and BA means the direction increases counterclockwise.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param direct Pointer to the variable where the direction (0 for AB, 1 for BA) will be stored.
     * @param timeout The timeout period for the operation (default is 100ms).
     *
     * @return Returns the status of the operation (1 for success, 0 for failure).
     */
    chain_status_t getEncoderABDirect(uint8_t id, uint8_t *direct, unsigned long timeout = 100);

    /**
     * @brief Gets the Chain_Encoder type code.
     *
     * This function returns the type code for the Chain_Encoder device, used to identify the device type.
     *
     * @return Returns the Chain_Encoder device type code.
     */
    uint16_t getEncoderTypeCode(void);

    /**
     * @brief Gets the encoder button press status.
     *
     * This function checks the current button press status of the encoder for the specified device in the chain.
     * The button press status is represented as a boolean value: true means the button is pressed,
     * and false means the button is not pressed.
     *
     * @param id The position of the device in the chain (starting from 1).
     *
     * @return Returns true if the button is pressed, false if the button is not pressed.
     */
    bool getEncoderButtonPressStatus(uint8_t id);

private:
};

#endif  // CHAIN_ENCODER_H