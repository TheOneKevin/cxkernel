/*
 * File:   panic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:11 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 07-Aug-2017 02:41:29 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

void __panic__();

#define PANIC(s)    \
{                   \
    for(int i = 0; s[i] != '\0'; i++) _ttys[_current_tty_id] -> console -> putc(s[i]); \
     __panic__();   \
}

#ifdef __cplusplus
}
#endif
