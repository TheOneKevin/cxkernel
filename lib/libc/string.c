/**
 * @file   string.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Thursday, October 11th 2018, 8:57:33 am
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T19:50:19-04:00
 */

#include "string.h"

EXPORT_SYMBOL(memcmp);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(strcat);
EXPORT_SYMBOL(strcpy);
EXPORT_SYMBOL(strdup);
EXPORT_SYMBOL(strcmp);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strncmp);

int memcmp(const void* buf1, const void* buf2, size_t len)
{
    const unsigned char* p1 = buf1, * p2 = buf2;
    while (len--)
        if (*p1 != *p2)
            return *p1 - *p2;
        else
            p1++, p2++;
    return 0;
}

void* memcpy(void* dest, const void* src, size_t len)
{
    #if ARCH_TYPE == ARCH_x86_32
        asm volatile ("rep movsb"
                      :
                      : "D" (dest), "S" (src), "c" (len)
                      : "memory");
        return dest;
    #else
        char* d = dest;
        const char* s = src;
        while (len--)
            *d++ = *s++;
        return dest;
    #endif
}

void* memset(void* dest, int c, size_t len)
{
    #if ARCH_TYPE == ARCH_x86_32
        asm volatile ("rep stosb"
                      :
                      : "a" (c), "D" (dest), "c" (len)
                      : "memory");
        return dest;
    #else
        unsigned char* ptr = dest;
        while (len-- > 0)
            *ptr++ = c;
        return dest;
    #endif
}

void* memmove(void* dest, void* src, size_t len)
{
    char* d = dest;
    char* s = src;
    if (d < s)
        while (len--)
            *d++ = *s++;
    else
    {
        char* lasts = s + (len - 1);
        char* lastd = d + (len - 1);
        while (len--)
            *lastd-- = *lasts--;
    }
    return dest;
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
    if (!str)
        return NULL;
    char* ret = NULL;//(char *) malloc(strlen(str) + 1);
    if (!ret)
        return NULL;
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

// See https://www.strchr.com/optimized_strlen_function for algorithm details
size_t strlen(const char* str)
{
    const char* ptr;
    const unsigned long int* longword_ptr;
    unsigned long int longword, himagic, lomagic;
    for (ptr = str; ((unsigned long int) ptr & (sizeof(longword) - 1)) != 0;
         ++ptr)
        if (*ptr == '\0')
            return ptr - str;
    longword_ptr = (unsigned long int *) ptr;
    himagic = 0x80808080L;
    lomagic = 0x01010101L;
    if (sizeof(longword) > 4)
    {
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    while (true)
    {
        longword = *longword_ptr++;

        if (((longword - lomagic) & ~longword & himagic) != 0)
        {
            const char* cp = (const char *) (longword_ptr - 1);
            if (cp[0] == '\0')
                return cp - str;
            if (cp[1] == '\0')
                return cp - str + 1;
            if (cp[2] == '\0')
                return cp - str + 2;
            if (cp[3] == '\0')
                return cp - str + 3;
            if (sizeof(longword) > 4)
            {
                if (cp[4] == '\0')
                    return cp - str + 4;
                if (cp[5] == '\0')
                    return cp - str + 5;
                if (cp[6] == '\0')
                    return cp - str + 6;
                if (cp[7] == '\0')
                    return cp - str + 7;
            }
        }
    }
}

// This function is in the public domain.
int strncmp(const char* s1, const char* s2, register size_t n)
{
    register unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char) *s1++;
        u2 = (unsigned char) *s2++;
        if (u1 != u2)
            return u1 - u2;
        if (u1 == '\0')
            return 0;
    }
    return 0;
}

// This function is in the public domain.
char* strstr (const char *s1, const char *s2)
{
    const char *p = s1;
    const size_t len = strlen (s2);
    for (; (p = strchr (p, *s2)) != 0; p++)
    {
        if (strncmp (p, s2, len) == 0)
	        return (char *)p;
    }
    return (0);
}

// This function is in the public domain.
char* strchr(const char *s, int c)
{
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *) s;
}