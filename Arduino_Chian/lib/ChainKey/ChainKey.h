/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_KEY_H
#define CHAIN_KEY_H

#include <ChainCommon.h>

/**
 * @brief CHAIN_KEY device type code.
 */
#define CHAIN_KEY_DEVICE_TYPE_CODE (0x0003)

/**
 * @brief CHAIN_KEY button press status code.
 */
#define CHAIN_KEY_BUTTON_PRESS_TYPE_CODE (0x0001)

class ChainKey : virtual public ChainCommon {
public:
    /**
     * @brief Gets the Chain_Key type code.
     *
     * This function returns the type code for the Chain_Key device, used to identify the device type.
     *
     * @return Returns the Chain_Key device type code.
     */
    uint16_t getKeyTypeCode(void);

    /**
     * @brief Gets the button press status of a specified device.
     *
     * This function checks the button press status of the device specified by the id.
     * It returns whether the button is pressed or not.
     *
     * @param id The ID of the device to query.
     *
     * @return Returns the button press status, true if the button is pressed, false otherwise.
     */
    bool getKeyButtonPressStatus(uint8_t id);

private:
};

#endif  // CHAIN_KEY_H