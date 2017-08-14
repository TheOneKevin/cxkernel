/*
 * File:   cpufeatset.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 09-Aug-2017 02:00:53 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 09-Aug-2017 04:32:12 PM
*/

#pragma once

// Based on AMD Architecture Manual Rev. 3.23 March 2017 (24594)
// and the Intel Developer's Manual Vol. 2A, July 2017
enum CPU_FEATSET
{
    CPU_FEAT_ECX_SSE3                   = 1 << 0,
    CPU_FEAT_ECX_PCMULQDQ               = 1 << 1,
    CPU_FEAT_ECX_DTES64                 = 1 << 2,
    CPU_FEAT_ECX_MONITOR                = 1 << 3,
    CPU_FEAT_ECX_DSCPL                  = 1 << 4,
    CPU_FEAT_ECX_VMX                    = 1 << 5,
    CPU_FEAT_ECX_SMX                    = 1 << 6,
    CPU_FEAT_ECX_EIST                   = 1 << 7,
    CPU_FEAT_ECX_TM2                    = 1 << 8,
    CPU_FEAT_ECX_SSSE3                  = 1 << 9,
    CPU_FEAT_ECX_CNXTID                 = 1 << 10,
    CPU_FEAT_ECX_SDBG                   = 1 << 11,
    CPU_FEAT_ECX_FMA                    = 1 << 12,
    CPU_FEAT_ECX_CMPXCHG16B             = 1 << 13,
    CPU_FEAT_ECX_XTPRUPDATECTRL         = 1 << 14,
    CPU_FEAT_ECX_PDCM                   = 1 << 15,
    // 16: Reserved
    CPU_FEAT_ECX_PCID                   = 1 << 17,
    CPU_FEAT_ECX_DCA                    = 1 << 18,
    CPU_FEAT_ECX_SSE4_1                 = 1 << 19,
    CPU_FEAT_ECX_SSE4_2                 = 1 << 20,
    CPU_FEAT_ECX_X2APIC                 = 1 << 21,
    CPU_FEAT_ECX_MOVBE                  = 1 << 22,
    CPU_FEAT_ECX_POPCNT                 = 1 << 23,
    CPU_FEAT_ECX_TSCDEADLINE            = 1 << 24,
    CPU_FEAT_ECX_AES                    = 1 << 25,
    CPU_FEAT_ECX_XSAVE                  = 1 << 26,
    CPU_FEAT_ECX_OSXSAVE                = 1 << 27,
    CPU_FEAT_ECX_AVX                    = 1 << 28,
    CPU_FEAT_ECX_F16C                   = 1 << 29,
    CPU_FEAT_ECX_RAND                   = 1 << 30,
    // On AMD platforms, its reserved for hypervisor to indicate guest status (RAV?)
    // On Intel platforms, this should always return 0

    CPU_FEAT_EDX_FPU                    = 1 << 0,
    CPU_FEAT_EDX_VME                    = 1 << 1,
    CPU_FEAT_EDX_DE                     = 1 << 2,
    CPU_FEAT_EDX_PSE                    = 1 << 3,
    CPU_FEAT_EDX_TSC                    = 1 << 4,
    CPU_FEAT_EDX_MSR                    = 1 << 5,
    CPU_FEAT_EDX_PAE                    = 1 << 6,
    CPU_FEAT_EDX_MCE                    = 1 << 7,
    CPU_FEAT_EDX_CMPXCHG8B              = 1 << 8,
    CPU_FEAT_EDX_APIC                   = 1 << 9,
    // 10: Reserved
    CPU_FEAT_EDX_SYSENTEREXIT           = 1 << 11,
    CPU_FEAT_EDX_MTRR                   = 1 << 12,
    CPU_FEAT_EDX_PGE                    = 1 << 13,
    CPU_FEAT_EDX_MCA                    = 1 << 14,
    CPU_FEAT_EDX_CMOV                   = 1 << 15,
    CPU_FEAT_EDX_PAT                    = 1 << 16,
    CPU_FEAT_EDX_PSE36                  = 1 << 17,
    CPU_FEAT_EDX_PSN                    = 1 << 18,
    CPU_FEAT_EDX_CLFLUSH                = 1 << 19,
    // 20: Reserved
    CPU_FEAT_EDX_DS                     = 1 << 21,
    CPU_FEAT_EDX_ACPI                   = 1 << 22,
    CPU_FEAT_EDX_MMX                    = 1 << 23,
    CPU_FEAT_EDX_FXSR                   = 1 << 24,
    CPU_FEAT_EDX_SSE                    = 1 << 25,
    CPU_FEAT_EDX_SSE2                   = 1 << 26,
    CPU_FEAT_EDX_HTT                    = 1 << 28,
    CPU_FEAT_EDX_TM                     = 1 << 29,
    CPU_FEAT_EX_I64                     = 1 << 30,
    CPU_FEAT_EDX_PBE                    = 1 << 31
};

#ifdef __cplusplus
extern "C" {
#endif

// C code goes here

#ifdef __cplusplus
}
#endif
