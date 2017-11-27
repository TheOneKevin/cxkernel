/*
 * File:   printk.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 02:10:34 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:25:00-05:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fflush(int stream);
void init_early_handles(void);

int kprintf(const char* format, ...);
int fprintf(char file, const char* format, ...);
int vsprintf(char* buf, const char* fmt, va_list args);
int ssprintf(void (*g)(char, char**), char* buf, const char* fmt, va_list args);
int sprintf(char* buf, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
