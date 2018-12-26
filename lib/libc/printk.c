/**
 * @file   printk.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 9:13:12 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-26T22:54:44-04:00
 */

// Have fun maintaining the code!

#include <stdio.h>

EXPORT_SYMBOL(printf);
EXPORT_SYMBOL(fprintf);
EXPORT_CTOR(init_early_handles, 101);

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
        case STREAM_LOG:
            *__stream_buffer[1] = 0; // Null terminator
#ifdef LOG_ENABLE
            console_log(&__stream_buffer_org[1][0]);
#else
            console_puts(&__stream_buffer_org[1][0]);
#endif
            __stream_buffer[1] = &__stream_buffer_org[1][0];
            break;
        case STREAM_OUT:
            *__stream_buffer[0] = 0; // Null terminator
            __stream_buffer[0] = &__stream_buffer_org[0][0]; // Reset buffer ptr
            while((*__stream_buffer[0]) && ((__stream_buffer[0] - &__stream_buffer_org[0][0]) < 512))
                console_emit(*__stream_buffer[0]++);
#ifdef LOG_ALL_OUTPUT
            console_log(&__stream_buffer_org[0][0]);
#endif
            __stream_buffer[0] = &__stream_buffer_org[0][0];
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

int printf(const char* format, ...)
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
