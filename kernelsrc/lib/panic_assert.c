/*
 * Filename: panic_assert.c
 * Author:   Kevin Dai
 * Email:   kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:00 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 11-Aug-2017 11:03:10 PM
*/

#include "lib/panic.h"
#include "lib/assert.h"

void __panic__()
{
    for(;;);
}

static char* assert1 = "Assertation '";
static char* assert2 = "' failed. \n";

void __assert__(bool condition, char* s, char* msg)
{
    if(!condition)
    {
        for(int i = 0; assert1[i] != '\0'; i++) _ttys[_current_tty_id] -> console -> putc(assert1[i]);
        for(int i = 0; s[i] != '\0'; i++) _ttys[_current_tty_id] -> console -> putc(s[i]);
        for(int i = 0; assert2[i] != '\0'; i++) _ttys[_current_tty_id] -> console -> putc(assert2[i]);
        for(int i = 0; msg[i] != '\0'; i++) _ttys[_current_tty_id] -> console -> putc(msg[i]);
        //__panic__();
    }
}
