/*
 * File:   string.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 05:10:16 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 10-Aug-2017 03:26:09 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

int    memcmp  (const void*, const void*, size_t);
void*  memcpy  (void*, const void*, size_t);
void*  memset  (void*, int, size_t);
char*  strcat  (char*, const char*);
char*  strcpy  (char*, const char*);
char*  strdup  (const char*);
int    strcmp  (const char*, const char*);
size_t strlen  (const char*);

#ifdef __cplusplus
}
#endif
