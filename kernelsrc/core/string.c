/*
 * File:   string.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 29-Jul-2017 05:09:51 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-08-29T22:07:57-04:00
*/

#include "lib/string.h"
#include "mm/malloc.h"

int memcmp(const void* buf1, const void* buf2, size_t len)
{
    const unsigned char *p1 = buf1, *p2 = buf2;
    while(len --)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

void* memcpy(void* dest, const void* src, size_t len)
{
#if ARCH_TYPE == PLATFORM_x86
    //asm volatile ("rep movsd" : : "D" (dest), "S" (src), "c" (len / 4) : "memory");
    //asm volatile ("rep movsb" : : "D" (dest + (len / 4) * 4), "S" (src + (len / 4) * 4), "c" (len - (len / 4) * 4) : "memory");
    asm volatile ("rep movsb" : : "D" (dest), "S" (src), "c" (len) : "memory");
    return dest;
#endif
}

void* memset(void* str, int c, size_t len)
{
#if ARCH_TYPE == PLATFORM_x86
    asm volatile ("rep stosl" : : "a" (c), "D" (str), "c" (len / 4) : "memory");
    asm volatile ("rep stosb" : : "a" (c), "D" (str + (len / 4) * 4), "c" (len - (len / 4) * 4) : "memory");
    return str;
#endif
}

char* strcat(char* dest, const char* src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

char* strcpy(char* dest, const char* src)
{
    return (char *) memcpy((void *) dest, (void *) src, strlen(src));
}

char* strdup(const char* str)
{
    if(!str) return NULL;
    char* ret = (char *) malloc(strlen(str) + 1);
    if(!ret) return NULL;
    ret[strlen(str)] = '\0';
    memcpy(ret, str, strlen(str));
    return ret;
}

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2))
        str1++, str2++;
    return *(const unsigned char *) str1 - *(const unsigned char *) str2;
}

size_t strlen(const char* str)
{
#if ARCH_TYPE == PLATFORM_x86
    const char* ptr;
    const uint32_t* dword_ptr;
    uint32_t himagic = 0x80808080L, lomagic = 0x01010101L;
    for(ptr = str; ((uint32_t) ptr & (sizeof(uint32_t) - 1)) != 0; ptr++)
        if(*ptr == '\0')
            return ptr - str;
    dword_ptr = (const uint32_t *) ptr;
    while(true)
    {
        uint32_t w = *dword_ptr++;
        uint32_t nullbyte = (w - lomagic) & ~w & himagic;
        if(nullbyte != 0)
        {
            const char* cp = (const char *) (dword_ptr - 1);
            if(cp[0] == '\0') return cp - str + 0;
            if(cp[1] == '\0') return cp - str + 1;
            if(cp[2] == '\0') return cp - str + 2;
            if(cp[3] == '\0') return cp - str + 3;
        }
    }
    return 0;
#endif
}
