/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_H
#define CHAIN_H

#include "ChainAngle.h"
#include "ChainEncoder.h"
#include "ChainJoystick.h"
#include "ChainKey.h"
#include "ChainToF.h"
#include "ChainUart.h"

class Chain : public ChainEncoder,
              public ChainAngle,
              public ChainJoystick,
              public ChainKey,
              public ChainUart,
              public ChainToF {

public:
private:
};

#endif // CHAIN_H