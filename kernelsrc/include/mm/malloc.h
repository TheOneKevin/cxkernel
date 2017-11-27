/*
 * File:   malloc.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-04T17:45:29-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-25T20:11:26-04:00
*/

#pragma once

#include "common.h"

#define PREFIX(func) func

#ifdef __cplusplus
extern "C" {
#endif

void liballoc_init(void*, size_t);

void    *PREFIX(malloc)(size_t);
void    *PREFIX(realloc)(void *, size_t);
void    *PREFIX(calloc)(size_t, size_t);
void     PREFIX(free)(void *);

#ifdef __cplusplus
}
#endif

#undef PREFIX
