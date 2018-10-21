/**
 * @file   console.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 12:31:41 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T15:48:42-04:00
 */

#include "console.h"
#include "platform/platform.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

void Console::putc(char c) { }
void Console::clear(void)  { }
char Console::getc(void)   { return 0; }

extern "C" void console_emit(char c)
{
    Platform::GetConsole().putc(c);
}

extern "C" void console_puts(char* s)
{
    while(*s) Platform::GetConsole().putc(*s++);
}

extern "C" char console_getc(void)
{
    return Platform::GetConsole().getc();
}
