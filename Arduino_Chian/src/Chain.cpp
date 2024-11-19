/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "Chain.h"

bool ChainBase::acquireMutex(void) {
  if (mutexLocked == false) {
    mutexLocked = true;
    return true;
  }
  return false;
}

void ChainBase::releaseMutex(void) { mutexLocked = false; }

void ChainBase::begin(HardwareSerial *serial, unsigned long baud,
                      uint32_t config, int8_t rxPin, int8_t txPin, bool invert,
                      unsigned long timeout_ms, uint8_t rxfifo_full_thrhd) {
  serialPort = serial;
  serialPort->begin(baud, SERIAL_8N1, rxPin, txPin, invert, timeout_ms,
                    rxfifo_full_thrhd);
}
void ChainBase::send(const char *buffer, size_t size) {
  if (serialPort) {
    serialPort->write(buffer, size);
  }
}