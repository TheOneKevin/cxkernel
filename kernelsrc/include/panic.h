/*
 * File:   panic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:11 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:27:00-05:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void __panic__(void);

#define PANIC(s)    \
{                   \
    fprintf(STREAM_ERR, s); \
     __panic__();   \
}

#ifdef __cplusplus
}
#endif
