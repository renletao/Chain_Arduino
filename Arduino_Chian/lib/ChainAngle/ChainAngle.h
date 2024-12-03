/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_ANGLE_H
#define CHAIN_ANGLE_H

#include <ChainCommon.h>

class ChainAngle : virtual public ChainCommon {

public:
  chain_status_t getAngle12Adc(uint8_t id, uint16_t *adcValue,
                               unsigned long timeout = 100);
  chain_status_t getAngle8Adc(uint8_t id, uint8_t *adcValue,
                              unsigned long timeout = 100);
  chain_status_t setAngleClockWise(uint8_t id, uint8_t direction,
                                   unsigned long timeout = 100);
  chain_status_t getAngleClockWise(uint8_t id, uint8_t *direction,
                                   unsigned long timeout = 100);

private:
};

#endif // CHAIN_ANGLE_H