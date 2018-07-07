/**
 * @file 
 * @copydoc i386-stub.c
 * @date Created on Sunday, May 27th 2018, 10:18:51 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T12:02:38-04:00
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