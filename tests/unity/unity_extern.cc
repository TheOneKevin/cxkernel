/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   unity_extern.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 27 2019, 5:09 PM
 */

#if ARCH_TYPE == ARCH_x86_32 || ARCH_TYPE == ARCH_x86_64
#include "platform/pc/serial.h"
using namespace pc;
extern "C" void __unityPutCharInternalPc(char c)
{
    serial::write(COM3, c);
}
extern "C" void __unityInitSerialPc()
{
    serial::init(COM3);
}
#endif