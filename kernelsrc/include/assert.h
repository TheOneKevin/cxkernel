/*
 * File:   assert.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 07-Aug-2017 02:29:16 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-19T18:32:19-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void __assert__(bool condition, char* s, char* msg, int l, char* f);
void __assert_hard__(bool condition, char* s, char* msg, int l, char* f);

#define ASSERT(c, s) __assert__(c, #c, s, __LINE__, __FILE__);
#define ASSERT_HARD(c, s) __assert_hard__(c, #c, s, __LINE__, __FILE__);

#ifdef __cplusplus
}
#endif
