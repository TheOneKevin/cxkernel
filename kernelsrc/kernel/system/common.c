/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "common.h"

void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}
void outw(uint16_t port, uint16_t value)
{
    asm volatile("outw %1, %0" : : "dN" (port), "a" (value));
}
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* memset(void* bufptr, int value, size_t size)
{
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

int memcmp(const void* aptr, const void* bptr, size_t size)
{
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;
    for (size_t i = 0; i < size; i++)
    {
        if (a[i] < b[i])
            return -1;
	else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

// Copy the NULL-terminated string src into dest
char *strcpy(char *dest, const char* src)
{
    char *ret = dest;
    while ((*dest++ = *src++))
        ;
    return ret;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

// Concatenate the NULL-terminated string src onto
// the end of dest and returns the concatenated version
char *strcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while ((*dest++ = *src++))
        ;
    return ret;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
	len++;
    return len;
}

void io_wait()
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

void halt()
{
    for(;;);
}

void cli()
{
    asm volatile("cli");
}

void sti()
{
    asm volatile("sti");
}