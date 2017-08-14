/*
 * File:   lib.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 04:11:28 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 01-Aug-2017 02:34:18 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

int strtol(char a[]);
char* convbase(long num, uint8_t base, bool small, bool isSigned);

#ifdef __cplusplus
}
#endif
