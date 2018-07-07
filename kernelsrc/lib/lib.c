/**
 * File:   lib.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:48:08-04:00
 * 
 * Sort of low cohesion library where random shit belongs
 */

#include "lib/lib.h"
#include "lib/string.h"

static const char _base1[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
static const char _base2[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

int strtol(char a[])
{
    int c = 0, sign = 0, offset = 0, n = 0;

    if (a[0] == '-')   // Handle negative integers
        sign = -1;

    if (sign == -1)   // Set starting position to convert
        offset = 1;
    else
        offset = 0;

    n = 0;

    for (c = offset; c < (signed) strlen(a); c++)
        n = n * 10 + a[c] - '0';

    if (sign == -1)
        n = -n;

    return n;
}

char* convbase(long num, char base, bool small, bool isSigned)
{
    char ret[23]; memset(ret, 0, 23); // 22 for 64 bit octal, +1 for EOL
    if(isSigned)
    {
        long divres = num < 0 ? -num : num; // Absolute value
        for(int i = 0; divres > 0; i++) // Do base conversion
        {
            if(small)
                ret[i] = _base1[divres % base];
            else
                ret[i] = _base2[divres % base];
            divres /= base;
        }
    }
    else
    {
        unsigned long divres = (unsigned long) num; // Absolute value
        for(int i = 0; divres > 0; i++) // Do base conversion
        {
            if(small)
                ret[i] = _base1[divres % base];
            else
                ret[i] = _base2[divres % base];
            divres /= base;
        }
    }

    unsigned char i = 0;
    unsigned char j = strlen(ret) - 1;

    // Reverse the string
    while (i < j)
    {
        char temp = ret[i];
        ret[i] = ret[j];
        ret[j] = temp;
        i++; j--;
    }

    return strdup(ret);
}
