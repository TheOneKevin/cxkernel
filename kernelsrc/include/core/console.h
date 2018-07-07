/**
 * @file   console.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:44:07-04:00
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
