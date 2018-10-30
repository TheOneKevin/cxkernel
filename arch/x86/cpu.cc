/**
 * @file   cpu.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 29th 2018, 7:21:36 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-29T19:26:09-04:00
 */

/*
 * File:   cpu.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 09-Aug-2017 02:10:45 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-03-16T10:12:50-04:00
*/

#include "arch/x86/cpu.h"

static uint32_t r0, r1, ecx, edx;

static inline int cpuid_string(int code, int where[4])
{
    asm volatile ("cpuid" : "=a" (*where), "=b" (*(where + 0)), "=d" (*(where + 1)), "=c" (*(where + 2)) : "a" (code));
    return (int) where[0];
}

void update_cpuid(void)
{
    cpuid(CPUID_CMD_FEAT ,&r0, &r1, &ecx, &edx);
} EXPORT_CTOR(update_cpuid);

const char* cpu_vend_string(void)
{
    static char vendor[16] = "            ";
    cpuid_string(0, (int *)(vendor));
    return vendor;
}

bool cpu_check_feat(enum CPU_FEATSET feat, enum CPU_FEATSET_REGS reg)
{   
    if(reg == CPU_FEATSET_REGS_ECX)
        return ecx & feat;
    else if(reg == CPU_FEATSET_REGS_EDX)
        return edx & feat;
    return false;
}
