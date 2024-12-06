/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_JOYSTICK_H
#define CHAIN_JOYSTICK_H

#include <ChainCommon.h>

#define CHAIN_JOYSTICK_TYPE_CODE (0x0004)
typedef enum {
    CHAIN_JOYSTICK_GET_16ADC                     = 0x30,  // Command to get 16-bit ADC values
    CHAIN_JOYSTICK_GET_8ADC                      = 0x31,  // Command to get 8-bit ADC values
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE       = 0x32,  // Command to get mapped range for X and Y axes
    CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE       = 0x33,  // Command to set mapped range for X and Y axes
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE = 0x34,  // Command to get 16-bit mapped values for X and Y
    CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE  = 0x35,  // Command to get 8-bit mapped values for X and Y
    CHAIN_JOYSTICK_BUTTON_PRESS                  = 0x40,  // Command for joystick button press event
} chain_joystick_cmd_t;                                   // Enumeration for joystick commands

class ChainJoystick : virtual public ChainCommon {
public:
    chain_status_t getJoystick16Adc(uint8_t id, uint16_t *, uint16_t *yAdcValue, unsigned long timeout = 100);
    chain_status_t getJoystick8Adc(uint8_t id, uint8_t *xAdcValue, uint8_t *yAdcValue, unsigned long timeout = 100);
    chain_status_t getJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size, unsigned long timeout = 100);
    chain_status_t setJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size, uint8_t *operationStatus,
                                          uint8_t saveToFlash = 0, unsigned long timeout = 100);
    chain_status_t getJoystickMappedInt16Value(uint8_t id, int16_t *xMapAdcValue, int16_t *yMapAdcValue,
                                               unsigned long timeout = 100);
    chain_status_t getJoystickMappedInt8Value(uint8_t id, int8_t *xMapAdcValue, int8_t *yMapAdcValue,
                                              unsigned long timeout = 100);
    uint16_t getJoystickTypeCode(void);

private:
};

#endif  // CHAIN_JOYSTICK_H