/**
 * @file   stdio.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, October 10th 2018, 10:20:45 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T16:17:10-04:00
 */

#pragma once
#include "common.h"

//! Writes format string to the log stream with module name
#define OS_LOG(fmt, ...) fprintf(STREAM_LOG, "[%-5s] " fmt, __MODULE__, ## __VA_ARGS__)
//! Writes format string to the debug log stream with module name
#ifdef DEBUG
#define OS_DBG(fmt, ...) fprintf(STREAM_LOG, "[%-5s] " fmt, __MODULE__, ## __VA_ARGS__);
#else
#define OS_DBG(fmt, ...)
#endif
//! Writes format string to the error stream with module name
#define OS_ERR(fmt, ...) fprintf(STREAM_ERR, "[%-5s] " fmt, __MODULE__, ## __VA_ARGS__)
//! Writes format string to the output stream with module name
#define OS_PRN(fmt, ...) fprintf(STREAM_OUT, "[%-5s] " fmt, __MODULE__, ## __VA_ARGS__)

__BEGIN_CDECLS

void fflush(int stream);
void init_early_handles(void);

/**
 * Prints a formatted string to the screen
 * @param  format  Format string
 * @param  ...     Arguments for format
 * @return         Length of string written
 */
int printf(const char* format, ...);

/**
 * Writes a formatted string to a kernel output stream
 * @param  file    Stream ID. See #define in system header.
 * @param  format  Format string
 * @param  ...     Arguments for format
 * @return         Length of string written
 */
int fprintf(char file, const char* format, ...);

/**
 * Formats a string
 * @param  buf     The buffer to contain the formatted string
 * @param  fmt     The string containing the formats
 * @param  args    The arguments for the formats
 * @return         The number of characters written
 */
int vsprintf(char* buf, const char* fmt, va_list args);

/**
 * Do not use.
 */
int ssprintf(void (*g)(char, char**), char* buf, const char* fmt, va_list args);

/**
 * Formats a string
 * @param  buf     The buffer to contain the formatted string
 * @param  fmt     The string containing the formats
 * @param  ...     The arguments for the formats
 * @return         The number of characters written
 */
int sprintf(char* buf, const char* fmt, ...);

__END_CDECLS