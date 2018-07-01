/*
 * File:   debug.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Sunday, May 27th 2018, 10:18:51 am
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-05-28T09:23:21-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void initialize_debugger(void);
void breakpoint(void);

#ifdef DEBUG
    #define BREAKPOINT() breakpoint();
#else
    #define BREAKPOINT()
#endif

#ifdef __cplusplus
}
#endif