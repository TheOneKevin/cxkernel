/**
 * @file   lib.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:48:16-04:00
 * 
 * Sort of low cohesion library where random shit belongs
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

int strtol(char a[]);
char* convbase(long num, char base, bool small, bool isSigned);

#ifdef __cplusplus
}
#endif
