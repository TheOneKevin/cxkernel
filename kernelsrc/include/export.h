/*
 * File:   export.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T17:27:45-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-30T18:54:20-04:00
*/

#pragma once

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
    extern typeof(sym) sym;                                 \
    static const char __kstrtab_##sym[]                     \
    __attribute__((section("kstrtab"), aligned(1)))         \
    = OS_SYMBOL_STR(sym);                                   \
    static const struct krnl_sym __ksymtab_##sym            \
    __attribute__((section("ksymtab" sec "+" #sym), used))  \
    = { (unsigned long)&sym, __kstrtab_##sym }

#define EXPORT_SYMBOL(sym) __EXPORT_SYMBOL(sym, "")
#define EXPORT_SYMBOL_GPL(sym) __EXPORT_SYMBOL(sym, "_gpl")
