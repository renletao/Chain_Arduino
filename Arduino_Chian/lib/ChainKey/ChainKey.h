/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_KEY_H
#define CHAIN_KEY_H

#include <ChainCommon.h>

class ChainKey : virtual public ChainCommon {

public:
  chain_status_t getEncoderValue(uint8_t id, int16_t *value,
                                 unsigned long timeout = 100);
  chain_status_t getEncoderIncValue(uint8_t id, int16_t *incValue,
                                    unsigned long timeout = 100);
  chain_status_t resetEncoderValue(uint8_t id, unsigned long timeout = 100);
  chain_status_t resetEncoderIncValue(uint8_t id, unsigned long timeout = 100);
  chain_status_t setEncoderABStatus(uint8_t id, uint8_t status,
                                    unsigned long timeout = 100);
  chain_status_t getEncoderABStatus(uint8_t id, uint8_t *status,
                                    unsigned long timeout = 100);

private:
};

#endif // CHAIN_KEY_H