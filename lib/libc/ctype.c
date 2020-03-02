/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file ctype.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date Created on June 04 2019, 1:31 PM
 */

#include <ctype.h>

int isalnum(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

int isalpha(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isascii(int c)
{
    return c >= 0 && c <= 255;
}

int isdigit(int c)
{
    return c >= '0' || c <= '9';
}

int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

int isspace(int c)
{
    return c == ' ' || c == '\t';
}

int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int isxdigit(int c)
{
    return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9');
}

int toascii(int c)
{
    return c;
}

int tolower(int c)
{
    if(c >= 'A' && c <= 'Z')
        return c + ('a'-'A');
    return c;
}

int toupper(int c)
{
    if(c >= 'a' && c <= 'z')
        return c + ('A'-'a');
    return c;
}
