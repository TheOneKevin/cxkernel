/*
 * File:   assert.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:16 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-23T19:12:00-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void __assert__(bool condition, char* s, char* msg);

#define ASSERT(c, s)        \
{                           \
    __assert__(c, #c, s);   \
}

#ifdef __cplusplus
}
#endif
