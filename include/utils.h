/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   utils.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 *
 * General purpose macros goes here
 */

#pragma once

#define NEST(x) x
#define IN ,
#define OF ,

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))
#define CHECK_MFLG(flags, mask) ((flags) & (mask))
#define STRINGIFY_HIDDEN(x) #x
#define STRINGIFY(x) STRINGIFY_HIDDEN(x)

#define GET_ARG2(arg1, arg2, ...) arg2
#define GET_ARG3(arg1, arg2, arg3, ...) arg3
#define GET_ARG4(arg1, arg2, arg3, arg4, ...) arg4

#define container_of(ptr, type, member) ({                  \
    const __typeof__( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#ifdef CONFIG_UNDERSCORE_SYMBOL_PREFIX
#define __OS_SYMBOL(x) _##x
#define __OS_SYMBOL_STR(x) "_" #x
#else
#define __OS_SYMBOL(x) x
#define __OS_SYMBOL_STR(x) #x
#endif

#define OS_SYMBOL(x) __OS_SYMBOL(x)
#define OS_SYMBOL_STR(x) __OS_SYMBOL_STR(x)

struct krnl_sym
{
    unsigned long val;
    const char* name;
};

#define __EXPORT_SYMBOL(sym, sec)                           \
    extern __typeof__(sym) sym;                             \
    static const char __kstrtab_##sym[]                     \
    __attribute__((section("kstrtab"), aligned(1)))         \
    = OS_SYMBOL_STR(sym);                                   \
    static const struct krnl_sym __ksymtab_##sym            \
    __attribute__((section("ksymtab" sec "+" #sym), used))  \
    = { (unsigned long)&sym, __kstrtab_##sym }

#ifdef __KERNEL__
    #define EXPORT_SYMBOL(sym)     __EXPORT_SYMBOL(sym, "")
    #define EXPORT_SYMBOL_GPL(sym) __EXPORT_SYMBOL(sym, "_gpl")
#else
    #define EXPORT_SYMBOL(sym)
    #define EXPORT_SYMBOL_GPL(sym)
#endif

#define __EXPORT_CTOR1(sym)     extern __typeof__(sym) sym __attribute__((constructor))
#define __EXPORT_CTOR2(sym, x)  extern __typeof__(sym) sym __CTOR(x)
#define __EXPORT_CTOR(...) GET_ARG3(__VA_ARGS__, __EXPORT_CTOR2, __EXPORT_CTOR1, )
#define EXPORT_CTOR(...) __EXPORT_CTOR(__VA_ARGS__)(__VA_ARGS__)

#ifdef __cplusplus
#define __BEGIN_CDECLS extern "C" {
#define __END_CDECLS }
#else
#define __BEGIN_CDECLS
#define __END_CDECLS
#endif

// Yeah!