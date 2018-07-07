/*
 * File:   log.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 2017-08-28T14:32:06-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-08-28T16:16:18-04:00
*/

#include "log.h"
#include "core/console.h"

extern console_t __internal_serial_cons;

void os_log(char* c) // TODO: Don't use serial in the future
{
    while(*c)
        __internal_serial_cons.putc(*c++);
}
