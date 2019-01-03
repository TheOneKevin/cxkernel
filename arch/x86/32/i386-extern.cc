/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   i386-extern.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on May 27 2018, 9:21 AM
 */

#include "platform/pc/serial.h"
#include "arch/x86/interrupts.h"

using namespace pc;

/* write a single character */
extern "C" void putDebugChar(char c)
{
    serial::write(COM2, c);
}

/* read and return a single char */
extern "C" int getDebugChar()
{
    char c = serial::read(COM2);
    return c;
}

/* assign an exception handler */
extern "C" void exceptionHandler(int a, unsigned int b)
{
    x86_32::idt::set_gate(a, b, 0x08, 0x8E);
}

extern "C" void initGdbSerial()
{
    serial::init(COM2);
}