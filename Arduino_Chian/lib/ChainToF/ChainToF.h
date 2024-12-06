/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_TOF_H
#define CHAIN_TOF_H

#include <ChainCommon.h>

#define CHAIN_TOF_TYPE_CODE (0x0005)
typedef enum {
    CHAIN_TOF_GET_DISTANCE = 0x50,  // Command to get the distance measurement
    CHAIN_TOF_SET_MODE     = 0x51,  // Command to set the mode of the TOF sensor
    CHAIN_TOF_GET_MODE     = 0x52,  // Command to get the current mode of the TOF sensor
} chain_tof_cmd_t;
class ChainToF : virtual public ChainCommon {
public:
    chain_status_t getTofDistance(uint8_t id, uint16_t *distance, unsigned long timeout = 100);
    chain_status_t setTofMode(uint8_t id, uint8_t mode, uint8_t *operationStatus, uint8_t saveToFlash = 0,
                              unsigned long timeout = 100);
    chain_status_t getTofMode(uint8_t id, uint8_t *mode, unsigned long timeout = 100);
    uint16_t getToFTypeCode(void);

private:
};

#endif  // CHAIN_TOF_H