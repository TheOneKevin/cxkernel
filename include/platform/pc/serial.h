/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   serial.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 */

#pragma once

#include "common.h"

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

namespace pc::serial
{
    void init(uint16_t);
    char read(uint16_t);
    void write(uint16_t, char);
}
