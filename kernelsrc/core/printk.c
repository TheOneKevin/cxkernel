/*
 * Filename: printk.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 31-Jul-2017 02:10:59 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 08-Aug-2017 12:15:58 PM
*/

// Have fun maintaining the code!

#include "lib/printk.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

void __printcharfuckme__(char c, char* d)
{
    _ttys[_current_tty_id] -> console -> putc(c);
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
    int ret = vsprintf(__printcharfuckme__, 0, format, args);
    va_end(args);
    return ret;
}

/**
 * Writes a formatted string to a kernel output stream
 * @param  file    Stream ID. See #define in system header.
 * @param  format  Format string
 * @param  VARARGS Arguments for format
 * @return         Length of string written
 */
int fprintf(uint8_t file, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = -1;
    switch (file)
    {
        case STREAM_NORM: ret = vsprintf(__printcharfuckme__, 0, format, args); break;
        case STREAM_INFO: break;
        case STREAM_ERR: break;
        default: ret = vsprintf(__printcharfuckme__, 0, format, args); break;
    }
    va_end(args);
    return ret;
}
