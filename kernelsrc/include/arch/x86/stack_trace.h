/*
 * File:   stack_trace.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:30:35 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-23T19:12:54-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void CallStackTrace(uint32_t aEBP, bool aBOOL);

#ifdef __cplusplus
}
#endif
