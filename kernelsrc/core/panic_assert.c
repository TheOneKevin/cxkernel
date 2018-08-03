/*
 * File:   panic_assert.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 07-Aug-2017 02:29:00 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-03-19T18:34:51-04:00
*/

#include "panic.h"
#include "assert.h"
#include "lib/printk.h"

void __panic__(char* f, int l)
{
    kprintf("Kernel Panicked in %s at line %d\n", f, l);
    for(;;);
}

void __assert__(bool condition, char* s, char* msg, int l, char* f)
{
    if(!condition)
        fprintf(STREAM_ERR, "Assert '%s' failed.\n    Line: %d at %s\n%s\n", s, l, f, msg);
}

void __assert_hard__(bool condition, char* s, char* msg, int l, char* f)
{
    if(!condition)
    {
        fprintf(STREAM_ERR, "Assert '%s' failed.\n    Line: %d at %s\n%s\n", s, l, f, msg);
        PANIC("Hard assert failed. Halting...");
    }
}
