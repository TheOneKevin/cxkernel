/*
 * This is the common file only the KERNEL uses.
 */

#include "system.h"
#include "cpufeatset.h"

//Copy n = size amount of src into dst
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size)
{
    uint8_t* dst = (uint8_t*)dstptr;
    const uint8_t* src = (const uint8_t*) srcptr;
    for (size_t i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}

//Set n = size amount of value at buf
void* memset(void* bufptr, uint32_t value, size_t size)
{
    uint8_t* buf = (uint8_t*)bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (uint8_t)value;
    return bufptr;
}

//Compare n = size amount of a to b
int memcmp(const void* aptr, const void* bptr, size_t size)
{
    const uint8_t* a = (const uint8_t*) aptr;
    const uint8_t* b = (const uint8_t*) bptr;
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

int strcmp(char *s1, char *s2)
{
    uint8_t c1, c2;
    while (1)
    {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2) return c1 < c2 ? -1 : 1;
        if (!c1) break;
    }

    return 0;
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
    while (str[len] != 0)
	   len++;
    return len;
}

void halt()
{
    for(;;);
}

inline void io_wait()
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ("outb %%al, $0x80" : : "a"(0));
    /* %%al instead of %0 makes no difference. Does the register need to be zeroed? */
}

inline void cli()
{
    asm volatile("cli");
}

inline void sti()
{
    asm volatile("sti");
}

inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

inline void outw(uint16_t port, uint16_t value)
{
    asm volatile("outw %1, %0" : : "dN" (port), "a" (value));
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

inline unsigned long read_cr0()
{
    unsigned long val;
    asm volatile ("mov %%cr0, %0" : "=r"(val));
    return val;
}

inline unsigned long read_cr1()
{
    unsigned long val;
    asm volatile ("mov %%cr1, %0" : "=r"(val));
    return val;
}

inline unsigned long read_cr2()
{
    unsigned long val;
    asm volatile ("mov %%cr2, %0" : "=r"(val));
    return val;
}

inline unsigned long read_cr3()
{
    unsigned long val;
    asm volatile ("mov %%cr3, %0" : "=r"(val));
    return val;
}

inline unsigned long read_cr4()
{
    unsigned long val;
    asm volatile ("mov %%cr4, %0" : "=r"(val));
    return val;
}
