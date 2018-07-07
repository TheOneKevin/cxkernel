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

#include "arch/x86/arch_common.h"
#include "arch/x86/cpu.h"

// Issue a CPUID command
static inline void cpuid(uint32_t code, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile ("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (code));
}

static inline int cpuid_string(int code, int where[4])
{
    asm volatile ("cpuid" : "=a" (*where), "=b" (*(where + 0)), "=d" (*(where + 1)), "=c" (*(where + 2)) : "a" (code));
    return (int) where[0];
}

const char* cpu_vend_string(void)
{
    static char vendor[16] = "            ";
    cpuid_string(0, (int *)(vendor));
    return vendor;
}

bool cpu_check_feat(enum CPU_FEATSET feat, enum CPU_FEATSET_REGS reg)
{
    uint32_t r0, r1, ecx, edx;
    cpuid(CPUID_CMD_FEAT ,&r0, &r1, &ecx, &edx);
    if(reg == CPU_FEATSET_REGS_ECX)
        return ecx & feat;
    else if(reg == CPU_FEATSET_REGS_EDX)
        return edx & feat;
    return false;
}
