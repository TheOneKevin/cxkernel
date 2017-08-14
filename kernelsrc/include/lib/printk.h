/*
 * File:   printk.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 02:10:34 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 04-Aug-2017 10:35:50 AM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

int kprintf(const char* format, ...);
int fprintf(uint8_t file, const char* format, ...);
int vsprintf(void (*g)(char, char*), char* buf, const char* fmt, va_list args);
int sprintf(char* buf, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
