/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_ANGLE_H
#define CHAIN_ANGLE_H

#include <ChainCommon.h>

#define CHAIN_ANGLE_TYPE_CODE (0x0002)
typedef enum {
    CHAIN_ANGLE_GET_12ADC = 0x30,             // CHAIN_ANGLE_GET_12ADC is a command to get the
                                              // latest 12-bit ADC value
    CHAIN_ANGLE_PGET_8ADC = 0x31,             // CHAIN_ANGLE_PGET_8ADC is a command to get the
                                              // latest 8-bit mapped ADC value
    CHAIN_ANGLE_SET_CLOCKWISE_STATUS = 0x32,  // CHAIN_ANGLE_SET_CLOCKWISE_STATUS is a command to set the
                                              // clockwise direction status
    CHAIN_ANGLE_GET_CLOCKWISE_STATUS = 0x33,  // CHAIN_ANGLE_GET_CLOCKWISE_STATUS is a command to get the current
                                              // clockwise direction status
} chain_angle_cmd_t;                          // chain_angle_cmd_t is the command type for angle
                                              // measurement operations

class ChainAngle : virtual public ChainCommon {
public:
    chain_status_t getAngle12Adc(uint8_t id, uint16_t *adcValue, unsigned long timeout = 100);
    chain_status_t getAngle8Adc(uint8_t id, uint8_t *adcValue, unsigned long timeout = 100);
    chain_status_t setAngleClockwise(uint8_t id, uint8_t direction, uint8_t *operationStatus, uint8_t saveToFlash = 0,
                                     unsigned long timeout = 100);
    chain_status_t getAngleClockwise(uint8_t id, uint8_t *direction, unsigned long timeout = 100);
    uint16_t getAngleTypeCode(void);

private:
};

#endif  // CHAIN_ANGLE_H