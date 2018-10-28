/**
 * @file   debug.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 27th 2018, 11:41:57 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T11:42:49-04:00
 */

#pragma once
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void initialize_debugger(void);
void breakpoint(void);

#ifdef __cplusplus
}
#endif
