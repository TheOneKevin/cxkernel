/*
 * File:   printk.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 02:10:34 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-27T16:05:09-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS_LOG(fmt, ...) fprintf(STREAM_LOG, "["__MODULE__"] " fmt, ## __VA_ARGS__)
#define OS_ERR(fmt, ...) fprintf(STREAM_ERR, "["__MODULE__"] " fmt, ## __VA_ARGS__)
#define OS_PRN(fmt, ...) fprintf(STREAM_OUT, "["__MODULE__"] " fmt, ## __VA_ARGS__)

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
