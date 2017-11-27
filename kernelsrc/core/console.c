/*
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T16:52:18-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:24:09-05:00
*/

#include "arch/arch_interface.h"
#include "core/console.h"

static console_t* console;

void console_init(void)
{
    console = arch_get_console();
}

void console_emit(char c)
{
    console -> putc(c);
}

void console_puts(char* s)
{
    while(*s) console -> putc(*s++);
}

char console_getc(void)
{
    return console -> getc();
}
