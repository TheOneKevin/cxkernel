/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   console.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 13 2018, 12:31 PM
 */

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "platform/console.h"
#include "platform.h"
using namespace platform;

void Console::putc(char c) { }
void Console::log(char c)  { }
void Console::clear(void)  { }
char Console::getc(void)   { return 0; }

extern "C" void console_emit(char c)
{
    platform::get_console().putc(c);
}

extern "C" void console_puts(char* s)
{
    while(*s) platform::get_console().putc(*s++);
}

extern "C" char console_getc(void)
{
    return platform::get_console().getc();
}

extern "C" void console_log(char* s)
{
    while(*s) platform::get_console().log(*s++);
}
