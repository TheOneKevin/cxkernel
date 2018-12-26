/**
 * @file   console.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 12:31:41 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:21:22-05:00
 */

#include "console.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

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
