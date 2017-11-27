/*
 * File:   lib.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 04:11:28 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T11:22:40-05:00
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
