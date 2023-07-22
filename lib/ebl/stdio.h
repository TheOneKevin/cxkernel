#pragma once
#include <stdarg.h>

namespace ebl {
    constexpr int COUT = 0;
    constexpr int CERR = 1;
    constexpr int CLOG = 2;
};

extern void console_emit(char c);
extern void console_puts(char const* c);
extern void console_log(char const* c);

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
 * Formats a string
 * @param  buf     The buffer to contain the formatted string
 * @param  fmt     The string containing the formats
 * @param  ...     The arguments for the formats
 * @return         The number of characters written
 */
int sprintf(char* buf, const char* fmt, ...);
