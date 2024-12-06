/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_ENCODER_H
#define CHAIN_ENCODER_H

#include <ChainCommon.h>

#define CHAIN_ENCODER_TYPE_CODE (0x0001)
typedef enum {
    CHAIN_ENCODER_GET_VALUE       = 0x10,  // Get encoder value
    CHAIN_ENCODER_GET_INC_VALUE   = 0x11,  // Get encoder increment value
    CHAIN_ENCODER_RESET_VALUE     = 0x13,  // Reset encoder value
    CHAIN_ENCODER_RESET_INC_VALUE = 0x14,  // Reset encoder increment value
    CHAIN_ENCODER_SET_AB_STATUS   = 0x15,  // Set AB status, 0->AB, 1->BA
    CHAIN_ENCODER_GET_AB_STATUS   = 0x16,  // Get AB status, 0->AB, 1->BA
    CHAIN_ENCODER_BUTTON_PRESS    = 0x40,  // Get button state
} chain_encoder_cmd_t;                     // Command types

class ChainEncoder : virtual public ChainCommon {
public:
    chain_status_t getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout = 100);
    chain_status_t getEncoderIncValue(uint8_t id, int16_t *incValue, unsigned long timeout = 100);
    chain_status_t resetEncoderValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t resetEncoderIncValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout = 100);
    chain_status_t setEncoderABDirect(uint8_t id, uint8_t direct, uint8_t *operationStatus, uint8_t saveToFlash = 0,
                                      unsigned long timeout = 100);
    chain_status_t getEncoderABDirect(uint8_t id, uint8_t *direct, unsigned long timeout = 100);
    uint16_t getEncoderTypeCode(void);

private:
};

#endif  // CHAIN_ENCODER_H