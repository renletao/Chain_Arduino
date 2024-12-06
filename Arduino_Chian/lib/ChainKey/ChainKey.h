/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_KEY_H
#define CHAIN_KEY_H

#include <ChainCommon.h>

#define CHAIN_KEY_TYPE_CODE (0x0003)
class ChainKey : virtual public ChainCommon {
public:
    uint16_t getKeyTypeCode(void);

private:
};

#endif  // CHAIN_KEY_H