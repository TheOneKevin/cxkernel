/**
 * @file   panic.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T09:20:21-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC diagnostic ignored "-Wwrite-strings"

/**
 * @warning It's recommended to use @ref PANIC instead.
 */
void __panic__(char* f, int l);

/**
 * Panics and then halts the system. Will print out line and file.
 */
#define PANIC(s)            \
{                           \
    fprintf(STREAM_ERR, s"\n"); \
    __panic__(__FILE__, __LINE__); \
}

#ifdef __cplusplus
}
#endif
