/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/kprintf.h"

static bool kprint(const char* data, size_t length)
{
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
    {
	if (kputc(bytes[i]) == '\0')
            return false;
    }
    return true;
}

int kputc(int ic)
{
    char c = (char) ic;
    console_write(&c);
    return ic;
}

int kprintf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
	size_t maxrem = INT_MAX - written;

	if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
		format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')   
                amount++;
            if (maxrem < amount)
                return -1;  // TODO: Set errno to EOVERFLOW.
            if (!kprint(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

	const char* format_begun_at = format++;
 
	if (*format == 'c')
        {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem)
		return -1;  // TODO: Set errno to EOVERFLOW.
            if (!kprint(&c, sizeof(c)))
		return -1;
            written++;
	}
        else if (*format == 's')
        {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len)
		return -1;  // TODO: Set errno to EOVERFLOW.
            if (!kprint(str, len))
		return -1;
            written += len;
	}
        else if(*format == 'u')
        {
            format++;
            uint32_t str = va_arg(parameters, uint32_t);
            size_t len = intlen(str);
            if(maxrem < len)
                return -1;
            console_write_dec(str);
            written += len;
        }
        else if(*format == 'X')
        {
            format++;
            uint32_t str = va_arg(parameters, uint32_t);
            size_t len = intlen(str);
            if(maxrem < len)
                return -1;
            console_write_hex(str);
            written += len;
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len)
            {
		// TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!kprint(format, len))
                return -1;
            written += len;
            format += len;
	}
    }
    
    va_end(parameters);
    return written;
}