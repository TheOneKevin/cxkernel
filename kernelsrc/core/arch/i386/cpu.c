/*
 * Filename: cpu.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 09-Aug-2017 02:10:45 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 09-Aug-2017 05:28:52 PM
*/

#include "arch/i386/common.h"
#include "arch/i386/cpufeatset.h"

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

// Detects which vendor your cpu is from
const char* cpu_detect()
{
    static char vendor[16] = "            ";
    cpuid_string(0, (int *)(vendor));
    return vendor;
}
