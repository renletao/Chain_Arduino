/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_JOYSTICK_H
#define CHAIN_JOYSTICK_H

#include <ChainCommon.h>

class ChainJoystick : virtual public ChainCommon {

public:
  chain_status_t getJoystick16Adc(uint8_t id, uint16_t *adcXValue,
                                  uint16_t *adcYValue,
                                  unsigned long timeout = 100);
  chain_status_t getJoystick8Adc(uint8_t id, uint8_t *adcValue,
                                 uint8_t *adcYValue,
                                 unsigned long timeout = 100);
  chain_status_t setJoystickMappedRange(uint8_t id, int16_t *buf, uint8_t size,
                                        unsigned long timeout = 100);
  chain_status_t getJoystickMappedRange(uint8_t id, int16_t *buf, uint8_t size,
                                        unsigned long timeout = 100);
  chain_status_t getJoystickMappedInt16Value(uint8_t id, int16_t *x, int16_t *y,
                                             unsigned long timeout = 100);
  chain_status_t getJoystickMappedInt8Value(uint8_t id, int8_t *x, int8_t *y,
                                            unsigned long timeout = 100);

private:
};

#endif // CHAIN_JOYSTICK_H