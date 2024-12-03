/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_TOF_H
#define CHAIN_TOF_H

#include <ChainCommon.h>

class ChainToF : virtual public ChainCommon {

public:
  chain_status_t getEncoderValue(uint8_t id, int16_t *value,
                                 unsigned long timeout = 100);
  chain_status_t getTofDistance(uint8_t id, uint16_t *distance,
                                unsigned long timeout = 100);
  chain_status_t setTofMode(uint8_t id, uint8_t mode,
                            unsigned long timeout = 100);
  chain_status_t getTofMode(uint8_t id, uint8_t *mode,
                            unsigned long timeout = 100);

private:
};

#endif // CHAIN_TOF_H