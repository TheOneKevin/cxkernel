/**
 * @file   compiler.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, October 14th 2018, 3:43:52 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-19T16:33:51-04:00
 */

#pragma once

#include "common.h"
#include "utils.h"

#define __NO_OPTIMIZE   __OPTIMIZE(0)
#define __OPTIMIZE(x)   __attribute__((optimize("O"#x)))
#define __PACKED        __attribute__((packed))
#define __UNUSED        __attribute__((__unused__))
#define __NOINLINE      __attribute__((noinline))
#define __ALIGNED(x)    __attribute__((aligned(x)))
#define __NORETURN      __attribute__((noreturn))
#define __WEAK          __attribute__((weak))
#define __PURE          __attribute__((pure))
#define __GET_CALLER(x) __builtin_return_address(0)
#define __GET_FRAME(x)  __builtin_frame_address(0)
#define DEPRECATED      __attribute__((deprecated))

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// Compiler checks
#if __STDC_HOSTED__ == 1
    #error "This is a fucking kernel. Please compile in a freestanding environment."
#endif

#ifndef __GNUC__
    #error "Please use GCC to compile this."
#endif

#ifdef __EXCEPTIONS
    #error "Anyone who want's to implement exception handling, feel free to create a PR."
#endif

#ifdef __GXX_RTTI
    #error "This compilation will fail (vtables etc)."
#endif

#if __SSP__ < 1
    //#warning "Looks like SSP is not enabled. Proceed at your own risk."
#endif

#ifdef __cplusplus
    #define __BEGIN_CDECLS extern "C" {
    #define __END_CDECLS   }
#else
    #define __BEGIN_CDECLS
    #define __END_CDECLS
#endif
