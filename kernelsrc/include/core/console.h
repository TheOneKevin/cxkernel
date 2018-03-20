/*
 * File:   console.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T16:52:30-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-18T13:45:05-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Basic console requirements
typedef struct
{
    char (*getc) (void);
    void (*putc) (const char c);
    void (*clear) (uint8_t bg);
} console_t;

void console_init(void);
void console_emit(char);
void console_puts(char*);
char console_getc(void);

#ifdef __cplusplus
}
#endif
