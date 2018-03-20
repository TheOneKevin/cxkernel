/*
 * File:   panic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:11 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-19T18:35:05-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void __panic__(char* f, int l);

#define PANIC(s)            \
{                           \
    fprintf(STREAM_ERR, s); \
    __panic__(__FILE__, __LINE__); \
}

#ifdef __cplusplus
}
#endif
