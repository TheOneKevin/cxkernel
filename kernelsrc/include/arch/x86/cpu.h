/**
 * @file   cpu.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T16:40:24-04:00
 */

#pragma once

#include "common.h"

/* ================================= VENDOR STRINGS ================================= */
// We need to concern ourselves with these two strings
#define CPUID_VENDOR_AMD        "AuthenticAMD"
#define CPUID_VENDOR_INTEL      "GenuineIntel"
// Some CPUs I never knew existed
#define CPUID_VENDOR_AMD_K5     "AMDisbetter!"
#define CPUID_VENDOR_CENTAUR    "CentaurHauls"
#define CPUID_VENDOR_CYRIX      "CyrixInstead"
#define CPUID_VENDOR_TRANSMETA1 "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA2 "GenuineTMx86"
#define CPUID_VENDOR_NATSEMICON "Geode by NSC"
#define CPUID_VENDOR_NEXGEN     "NexGenDriven"
#define CPUID_VENDOR_RISE       "RiseRiseRise"
#define CPUID_VENDOR_SIS        "SiS SiS SiS "
#define CPUID_VENDOR_UMC        "UMC UMC UMC "
#define CPUID_VENDOR_VIA        "VIA VIA VIA "
#define CPUID_VENDOR_VORTEX     "Vortex86 SoC"
// Virtual machines
#define CPUID_VENDOR_KVM        "KVMKVMKVM"
#define CPUID_VENDOR_MSHYPERV   "Microsoft HV"
#define CPUID_VENDOR_PARALLELS  " lrpepyh vr" //! haha should be "prl hyperv" but some idiot messed up
#define CPUID_VENDOR_VMWARE     "VMwareVMWare"
#define CPUID_VENDOR_XENVMM     "XenVMMXenVMM"

/* ================================= CPUID COMMANDS ================================= */
#define CPUID_CMD_VENDOR 0
#define CPUID_CMD_FEAT   1

//! Based on AMD Architecture Manual Rev. 3.23 March 2017 (24594)
//! and the Intel Developer's Manual Vol. 2A, July 2017
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
    // 30: Reserved
    CPU_FEAT_EDX_PBE                    = 1 << 31
};

enum CPU_XFEATSET
{
    CPU_XFEAT_ECX_LAHFLM        = 1 << 0,
    CPU_XFEAT_ECX_CMPLEGACY     = 1 << 1,
    CPU_XFEAT_ECX_SVM           = 1 << 2,
    CPU_XFEAT_ECX_XAPIC         = 1 << 3,
    CPU_XFEAT_ECX_ALTCR8        = 1 << 4,
    CPU_XFEAT_ECX_LZCNT         = 1 << 5,
    CPU_XFEAT_ECX_SSE4A         = 1 << 6,
    CPU_XFEAT_ECX_MisAlignSSE   = 1 << 7,
    CPU_XFEAT_ECX_PREFETCHW     = 1 << 8,
    CPU_XFEAT_ECX_OSVW          = 1 << 9,
    CPU_XFEAT_ECX_IBS           = 1 << 10,
    CPU_XFEAT_ECX_XOP           = 1 << 11,
    CPU_XFEAT_ECX_SKINIT        = 1 << 12,
    CPU_XFEAT_ECX_WDT           = 1 << 13,
    // 14: Reserved
    CPU_XFEAT_ECX_LWP           = 1 << 14,
    CPU_XFEAT_ECX_FMA4          = 1 << 15,
    // 17-20: Reserved
    CPU_XFEAT_ECX_TBM           = 1 << 21,
    CPU_XFEAT_ECX_TopoExt       = 1 << 22,
    CPU_XFEAT_ECX_PerfCtrExCore = 1 << 23,
    CPU_XFEAT_ECX_PerfCtrExNB   = 1 << 24,
    // 25: Reserved
    CPU_XFEAT_ECX_DataBRKPNTx   = 1 << 26,
    CPU_XFEAT_ECX_PerfTSC       = 1 << 27,
    // 28-31: Reserved

    CPU_XFEAT_EDX_FPU           = 1 << 0,
    CPU_XFEAT_EDX_VME           = 1 << 1,
    CPU_XFEAT_EDX_DE            = 1 << 2,
    CPU_XFEAT_EDX_PSE           = 1 << 3,
    CPU_XFEAT_EDX_TSC           = 1 << 4,
    CPU_XFEAT_EDX_MSR           = 1 << 5,
    CPU_XFEAT_EDX_PAE           = 1 << 6,
    CPU_XFEAT_EDX_MCE           = 1 << 7,
    CPU_XFEAT_EDX_CMPXCHG8B     = 1 << 8,
    CPU_XFEAT_EDX_APIC          = 1 << 9,
    // 10: Reserved
    CPU_XFEAT_EDX_SYSCALLRET64  = 1 << 11,
    CPU_XFEAT_EDX_MTRR          = 1 << 12,
    CPU_XFEAT_EDX_PGE           = 1 << 13,
    CPU_XFEAT_EDX_MCA           = 1 << 14,
    CPU_XFEAT_EDX_CMOV          = 1 << 15,
    CPU_XFEAT_EDX_PAT           = 1 << 16,
    CPU_XFEAT_EDX_PSE36         = 1 << 17,
    // 18-19: Reserved
    CPU_XFEAT_EDX_NX            = 1 << 20, //!< Also known as XD on Intel Platforms
    // 21: Reserved
    CPU_XFEAT_EDX_MMXExt        = 1 << 22,
    CPU_XFEAT_EDX_MMX           = 1 << 23,
    CPU_XFEAT_EDX_FXSR          = 1 << 24,
    CPU_XFEAT_EDX_FFXSR         = 1 << 25,
    CPU_XFEAT_EDX_1GBPage       = 1 << 26,
    CPU_XFEAT_EDX_RDTSCP        = 1 << 27,
    // 28: Reserved
    CPU_XFEAT_EDX_AMD64         = 1 << 29, //!< IA64
    CPU_XFEAT_EDX_3DNowExt      = 1 << 30,
    CPU_XFEAT_EDX_3DNow         = 1 << 31,
};

enum CPU_FEATSET_REGS
{
    CPU_FEATSET_REGS_ECX,
    CPU_FEATSET_REGS_EDX
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fetches the CPU vendor string i.e., GenuineIntel
 * @return Pointer to read-only vendor string
 */
const char* cpu_vend_string(void);

/**
 * Checks if a feature flag has been set.
 * @param  feat The feature flag to be checked
 * @param  reg  The register the flag is in (CPU_FEATSET_REGS_ECX or CPU_FEATSET_REGS_EDX)
 * @return      True if flag is set, and false if not set.
 */
bool cpu_check_feat(enum CPU_FEATSET feat, enum CPU_FEATSET_REGS reg);

#ifdef __cplusplus
}
#endif
