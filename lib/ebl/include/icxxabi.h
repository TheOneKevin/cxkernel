/**
 * @file   icxxabi.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 1:37:25 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T15:49:25-04:00
 * 
 * Functions in this file are implemented as specified in the Intel Itanium
 * C++ standard. These functions are required by GCC. This file should at least
 * be included in the main kernel.cc file regardless of architecture.
 */

#pragma once

#include "common.h"
#define ATEXIT_MAX_FUNCS 128

typedef struct
{
    void (*destructor_func)(void*);
    void* obj_ptr;
    void* dso_handle;
} atexit_func_entry_t;

__BEGIN_CDECLS

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
void __cxa_finalize(void *f);
void __cxa_pure_virtual();

__END_CDECLS