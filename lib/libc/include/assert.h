/**
 * @file   assert.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T09:19:39-04:00
 */

#pragma once

#ifndef libm
    #include "common.h"
#else
    #define __FILENAME__ __FILE__
    #define __BEGIN_CDECLS
    #define __END_CDECLS
    #include <stdbool.h>
#endif

__BEGIN_CDECLS

#pragma GCC diagnostic ignored "-Wwrite-strings"

void __assert__(bool condition, char* s, char* msg, int l, char* f);
void __assert_hard__(bool condition, char* s, char* msg, int l, char* f);

#define ASSERT(c, s) __assert__(c, #c, s, __LINE__, __FILENAME__);
#define ASSERT_HARD(c, s) __assert_hard__(c, #c, s, __LINE__, __FILENAME__);

#define assert(x) ASSERT(x, "")

__END_CDECLS