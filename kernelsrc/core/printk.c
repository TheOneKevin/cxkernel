/*
 * File:   printk.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 02:10:59 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T21:12:35-04:00
*/

// Have fun maintaining the code!

#include "lib/printk.h"
#include "core/console.h"
#include "log.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================================= //
// Early definitions
static char  __stream_buffer_org[2][512];
static char* __stream_buffer[] = { &__stream_buffer_org[0][0], &__stream_buffer_org[1][0] };

static void _stream_log(char c, char** buf)
{
    *__stream_buffer[1]++ = c;
    if(c == '\n' || (__stream_buffer[1] - &__stream_buffer_org[1][0]) > 512)
        fflush(STREAM_LOG);
}

static void _stream_out(char c, char** buf)
{
    *__stream_buffer[0]++ = c;
    if(c == '\n' || (__stream_buffer[0] - &__stream_buffer_org[0][0]) > 512)
        fflush(STREAM_OUT);
}

static void _stream_err(char c, char** unused)
{
    console_emit(c);
}

void fflush(int fd)
{
    switch(fd)
    {
        case STREAM_OUT:
            *__stream_buffer[0] = 0; // Null terminator
            __stream_buffer[0] = &__stream_buffer_org[0][0]; // Reset buffer ptr
            while((*__stream_buffer[0]) && ((__stream_buffer[0] - &__stream_buffer_org[0][0]) < 512))
                console_emit(*__stream_buffer[0]++);
            os_log(&__stream_buffer_org[0][0]);
            __stream_buffer[0] = &__stream_buffer_org[0][0];
            break;
        case STREAM_LOG:
            *__stream_buffer[1] = 0; // Null terminator
            os_log(&__stream_buffer_org[1][0]);
            __stream_buffer[1] = &__stream_buffer_org[1][0];
            break;
    }
}
// ============================================================================================= //

void (*__stream_out)(char, char**);
void (*__stream_log)(char, char**);
void (*__stream_err)(char, char**);

/**
 * Initializes the early handles required for a functioning printf
 */
void init_early_handles(void)
{
    __stream_out = _stream_out;
    __stream_log = _stream_log;
    __stream_err = _stream_err;
}

/**
 * Prints a formatted string to the screen
 * @param  format  Format string
 * @param  VARARGS Arguments for format
 * @return         Length of string written
 */
int kprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = ssprintf(__stream_out, 0, format, args);
    va_end(args);
    return ret;
}

static inline void f(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    ssprintf(__stream_log, 0, format, args);
    va_end(args);
}

/**
 * Writes a formatted string to a kernel output stream
 * @param  file    Stream ID. See #define in system header.
 * @param  format  Format string
 * @param  VARARGS Arguments for format
 * @return         Length of string written
 */
int fprintf(char file, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = -1;
    switch (file)
    {
        // TODO: Implement actual real streams for these
        case STREAM_OUT: ret = ssprintf(__stream_out, 0, format, args); break;
        case STREAM_LOG: ret = ssprintf(__stream_log, 0, format, args); break;
        case STREAM_ERR: ret = ssprintf(__stream_err, 0, format, args); break;
        default:         ret = ssprintf(__stream_out, 0, format, args); break;
    }
    va_end(args);
    return ret;
}
