/*
 * File:   assert.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:16 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 07-Aug-2017 02:51:43 PM
*/

#pragma once

#include "system.h"

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
