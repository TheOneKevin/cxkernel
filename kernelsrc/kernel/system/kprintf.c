/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/kprintf.h"
#include "system/tdisplay.h"

static bool kprint(const char* data, size_t length)
{
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
    {
	if (kputc(bytes[i]) == 0)
            return false;
    }
    return true;
}

int kputc(int ic)
{
    console_putc(ic);
    return ic;
}

int kprintf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != 0)
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
		return -1;
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
		return -1;
            if (!kprint(str, len))
		return -1;
            written += len;
	}
        else if(*format == 'u')
        {
            format++;
            uint32_t str = va_arg(parameters, uint32_t);
            // We are relying on the fact here that no stupid
            // person is going to add a \n inside our console_write_dec thing
            // I mean why would you do it anyway?
            uint32_t laspos = (uint32_t)(y * 80 + x);
            console_write_dec(str);
            written += (uint32_t)(laspos - (y * 80 + x));
        }
        else if(*format == 'X')
        {
            format++;
            uint32_t str = va_arg(parameters, uint32_t);
            uint32_t laspos = (uint32_t)(y * 80 + x);
            console_write_hex(str);
            written += (uint32_t)(laspos - (y * 80 + x));
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len)
            {
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