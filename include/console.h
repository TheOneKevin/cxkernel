/**
 * @file   console.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 6:08:11 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T08:51:30-04:00
 */

#pragma once
#include "common.h"

// NB: Included by printk.c

#ifdef __cplusplus
class Console
{
public:
    virtual void putc(char c);
    virtual void clear(void);
    virtual char getc(void);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

void console_emit(char);
void console_puts(char*);
char console_getc(void);

#ifdef __cplusplus
}
#endif
