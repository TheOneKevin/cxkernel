/*
 * File:   cpu.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 09-Aug-2017 02:10:45 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:39:32-05:00
*/

#include "arch/x86/arch_common.h"
#include "arch/x86/cpufeatset.h"
#include "arch/x86/cpu.h"

// Issue a CPUID command
static inline void cpuid(uint32_t code, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile ("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (code));
}

static inline int cpuid_string(int code, int where[4])
{
    asm volatile ("cpuid" : "=a" (*where), "=b" (*(where + 0)), "=d" (*(where + 1)), "=c" (*(where + 2)) : "a" (code));
    return (int) where[0];
}

/**
 * Fetches the CPU vendor string i.e., GenuineIntel
 * @return Pointer to read-only vendor string
 */
const char* cpu_vend_string(void)
{
    static char vendor[16] = "            ";
    cpuid_string(0, (int *)(vendor));
    return vendor;
}

/**
 * Checks if a feature flag has been set.
 * @param  feat The feature flag to be checked
 * @param  reg  The register the flag is in (CPU_FEATSET_REGS_ECX or CPU_FEATSET_REGS_EDX)
 * @return      True if flag is set, and false if not set.
 */
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
