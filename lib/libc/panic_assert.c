/**
 * @file   panic_assert.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on 07-Aug-2017 02:29:00 PM
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-28T23:44:52-04:00
 */

#include <stdio.h>
#include <panic.h>
#include <assert.h>

__NORETURN void __panic__(char* f, int l)
{
    printf("Kernel Panicked in %s at line %d\n", f, l);
    for(;;);
}

void __assert__(bool condition, char* s, char* msg, int l, char* f)
{
    if(!condition)
        fprintf(STREAM_ERR, "Assertation failed.\n'%s'\n    Line: %d at %s\n%s\n", s, l, f, msg);
}

void __assert_hard__(bool condition, char* s, char* msg, int l, char* f)
{
    if(!condition)
    {
        fprintf(STREAM_ERR, "Assertation failed.\n'%s'\n    Line: %d at %s\n%s\n", s, l, f, msg);
        PANIC("Hard assert failed. Halting...");
    }
}
