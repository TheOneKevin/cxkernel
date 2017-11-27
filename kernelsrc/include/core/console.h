/*
 * File:   console.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T16:52:30-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:24:23-05:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef char (*vHookA) (void);
typedef void (*vHookB) (const char c);
typedef void (*vHookC) (uint8_t bg);

// Basic console requirements
typedef struct
{
    vHookA getc;
    vHookB putc;
    vHookC clear;
} console_t;

void console_init(void);
void console_emit(char);
void console_puts(char*);
char console_getc(void);

#ifdef __cplusplus
}
#endif
