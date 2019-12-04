/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   compiler.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 14 2018, 3:43 PM
 */

#pragma once

#include "utils.h"
#include "config.h"

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#define __NO_OPTIMIZE   __OPTIMIZE(0)
#define __OPTIMIZE(x)   __attribute__((optimize("O"#x)))
#define __PACKED        __attribute__((packed))
#define __UNUSED        __attribute__((__unused__))
#define __NOINLINE      __attribute__((noinline))
#define __ALIGNED(x)    __attribute__((aligned(x)))
#define __NORETURN      __attribute__((noreturn))
#define __WEAK          __attribute__((weak))
#define __PURE          __attribute__((pure))
#define __CTOR(x)       __attribute__((constructor(x)))
#define __GET_CALLER(x) __builtin_return_address(0)
#define __GET_FRAME(x)  __builtin_frame_address(0)
#define DEPRECATED(x)   __attribute__((deprecated(x)))
#ifdef CUSTOM_FILENAME
    #define __FILENAME__ __FILENAME_CUSTOM__
#else
    #define __FILENAME__ __FILE__
#endif

#define IGNORE(x) GCC diagnostic ignored #x

#define unlikely(x) __builtin_expect((x), 0)
#define likely(x)   __builtin_expect((x), 1)
#define alloca(x)   __builtin_alloca((x))

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
    #define TYPEOF(x) __typeof__(x)
#else
    #define __BEGIN_CDECLS
    #define __END_CDECLS
    #define TYPEOF(x) typeof(x)
#endif
