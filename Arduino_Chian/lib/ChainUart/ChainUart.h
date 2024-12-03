/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_UART_H
#define CHAIN_UART_H

#include <ChainCommon.h>

class ChainUart : virtual public ChainCommon {

public:
  chain_status_t chianUartI2cInit(uint8_t id, uint8_t *status,
                                  unsigned long timeout = 100);
  chain_status_t chainUartI2cRead(uint8_t id, uint8_t i2cAddr,
                                  uint8_t readLength, uint8_t *buffer,
                                  uint8_t *status, unsigned long timeout = 100);
  chain_status_t chainUartI2cWrite(uint8_t id, uint8_t i2cAddr, uint8_t write,
                                   uint8_t *buffer, uint8_t *status,
                                   unsigned long timeout = 100);
  chain_status_t chainUartI2cMemRead(uint8_t id, uint8_t i2cAddr,
                                     uint16_t regAddr, uint8_t regLength,
                                     uint8_t readLength, uint8_t *buffer,
                                     uint8_t *status,
                                     unsigned long timeout = 100);
  chain_status_t chainUartI2cMemWrite(uint8_t id, uint8_t i2cAddr,
                                      uint16_t regAddr, uint8_t regLength,
                                      uint8_t writeLength, uint8_t *buffer,
                                      uint8_t *status,
                                      unsigned long timeout = 100);
  chain_status_t getChainUartI2cDeviceAddr(uint8_t id, uint8_t *i2cAddrNums,
                                           uint8_t *buffer, uint8_t *status,
                                           unsigned long timeout = 100);
  chain_status_t chainUartOutputInit(uint8_t id, uint8_t gpio,
                                     uint8_t gpioLevel, uint8_t gpioMode,
                                     uint8_t gpioUp, uint8_t gpioSpeed,
                                     uint8_t *status,
                                     unsigned long timeout = 100);
  chain_status_t chainUartInputInit(uint8_t id, uint8_t gpio, uint8_t gpioUp,
                                    uint8_t *status,
                                    unsigned long timeout = 100);
  chain_status_t chainUartReadLevel(uint8_t id, uint8_t gpio, uint8_t *status,
                                    unsigned long timeout = 100);
  chain_status_t chainUartNvicInit(uint8_t id, uint8_t gpio, uint8_t gpioUp,
                                   uint8_t triggerMode, uint8_t *status,
                                   unsigned long timeout = 100);
  chain_status_t chainUartAdcInit(uint8_t id, uint8_t adcChannel1,
                                  uint8_t adcChannel2, uint8_t *status,
                                  unsigned long timeout = 100);
  chain_status_t getAdcValue(uint8_t id, uint8_t *adcChannelNums,
                             uint8_t *buffer, uint8_t *status,
                             unsigned long timeout = 100);

private:
};

#endif // CHAIN_UART_H