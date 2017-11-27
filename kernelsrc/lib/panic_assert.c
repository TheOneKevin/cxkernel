/*
 * File:   panic_assert.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:00 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:26:55-05:00
*/

#include "panic.h"
#include "lib/printk.h"
#include "assert.h"

void __panic__(void)
{
    for(;;);
}

void __assert__(bool condition, char* s, char* msg)
{
    if(!condition)
        fprintf(STREAM_ERR, "Assertation '%s' failed.\n%s\n", s, msg);
}
