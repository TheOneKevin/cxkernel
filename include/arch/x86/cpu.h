/**
 * @file   cpu.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-29T22:19:52-04:00
 * 
 * I hand wrote this. It was painful.
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
//! Based on AMD Architecture Manual Vol. 3 Rev. 3.23 March 2017 (Pub 24594)
//! and the Intel Developer's Manual Vol. 2A, July 2017
//! See 3-190 and 3-191 of the Intel Developer's Manual Vol. 2A
//! See Appendix E of the AMD Manual
enum cpuid_cmd
{
    CPUID_BASE       = 0x0, //!< Basic CPUID Information
    CPUID_MODEL_FEAT = 0x1,
    CPUID_CACHE_V0   = 0x2,
    CPUID_CACHE_V1   = 0x3,
    //! CPUID leaves above 2 and below 80000000H are visible only when
    //! IA32_MISC_ENABLE[bit 22] has its default value of 0.
    CPUID_CACHE3    = 0x4, //!< Deterministic Cache Parameters Leaf
    CPUID_MWAIT     = 0x5, //!< MONITOR/MWAIT Leaf
    CPUID_THERM     = 0x6, //!< Thermal and Power Management Leaf
    CPUID_EXT_FEAT  = 0x7, //!< Structured Extended Feature Flags Enumeration Leaf (Output depends on ECX input value)
    CPUID_PERF      = 0xA, //!< (INTEL) Architectural Performance Monitoring Leaf
    CPUID_TOPOLOGY  = 0xB, //!< (INTEL) Extended Topology Enumeration Leaf 
    CPUID_XSAVE     = 0xD, //!< Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0)
    
    // Extended CPUID commands
    CPUID_EXT_BASE = 0x80000000,
    CPUID_EXT_MODEL_FEAT = 0x80000001,
    CPUID_EXT_CACHE = 0x80000006,
    CPUID_EXT_ADDR_SIZE = 0x80000008,
}

//! CPUID result structure (4 registers)
struct cpuid_leaf
{
    //! All 4 registers are 32 bits regardless of CPU mode
    uint32_t a, b, c, d;
}

//! CPUID query structure
struct cpuid_bit
{
    enum cpuid_cmd leaf;
    uint8_t word, bit;
} __PACKED;

#define CPUID_BIT(l, w, b) (struct cpuid_bit){ (enum cpuid_cmd)(l), (w), (b) }

/* ================================= CPUID FEATURES ================================= */
/* Add feature bits here, use CPUID_BIT(cpuid command, register (eax -> 0, etc), bit) */
// Here, we add flags supported by both AMD and INTEL. For futher details,
// consult the Intel and AMD manuals (3-213, 3-208 and Appendix E respectively)
#define x86_FEATURE_SSE3                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 0) //!< SSE3 Extensions
#define x86_FEATURE_MONITOR                CPUID_BIT(CPUID_MODEL_FEAT, 2, 3) //!< MONITOR/MWAIT
#define x86_FEATURE_SSSE3                  CPUID_BIT(CPUID_MODEL_FEAT, 2, 9) //!< Supplemental SSE3 instruction support
#define x86_FEATURE_SSE4_1                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 19) //!< SSE4.2 instruction support
#define x86_FEATURE_SSE4_2                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 20) //!< SSE4.1 instruction support
#define x86_FEATURE_AES                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 25) //!< AES instruction support
#define x86_FEATURE_XSAVE                  CPUID_BIT(CPUID_MODEL_FEAT, 2, 26) //!< XSAVE
#define x86_FEATURE_OSXSAVE                CPUID_BIT(CPUID_MODEL_FEAT, 2, 27) //!< OSXSAVE
#define x86_FEATURE_AVX                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 28) //!< AVX
#define x86_FEATURE_RDRAND                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 30) //!< RDRAND
#define x86_FEATURE_FPU                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 0) //!< x87 floating point unit on-chip
#define x86_FEATURE_VME                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 1) //!< Virtual 8086 extensions
#define x86_FEATURE_PSE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 3) //!< Page Size Extensions
#define x86_FEATURE_PAE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 6) //!< Page Address Extensions
#define x86_FEATURE_APIC                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 9) //!< APIC on Chip
#define x86_FEATURE_SYSENTEREXIT           CPUID_BIT(CPUID_MODEL_FEAT, 3, 11) //!< SYSENTER/EXIT
#define x86_FEATURE_PGE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 13) //!< PTE Global Bit
#define x86_FEATURE_PSE36                  CPUID_BIT(CPUID_MODEL_FEAT, 3, 17) //!< Page Size Extension
#define x86_FEATURE_CLFLUSH                CPUID_BIT(CPUID_MODEL_FEAT, 3, 19) //!< CFLUSH
#define x86_FEATURE_MMX                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 23) //!< MMX Technology
#define x86_FEATURE_FXSR                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 24) //!< FXSAVE/FXRSTOR
#define x86_FEATURE_SSE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 25) //!< SSE Extensions
#define x86_FEATURE_SSE2                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 26) //!< SSE2 Extensions
#define x86_FEATURE_TM                     CPUID_BIT(CPUID_MODEL_FEAT, 3, 29) //!< Thermal monitor
#define x86_FEATURE_HTT                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 28) //!< Hyper threading
#define x86_FEATURE_SYSCALL                CPUID_BIT(CPUID_EXT_MODEL_FEAT, 3, 11) //!< SYSCALL/SYSRET available in 64-bit mode.
#define x86_FEATURE_NX                     CPUID_BIT(CPUID_EXT_MODEL_FEAT, 3, 20) //!< Execute Disable Bit available
#define x86_FEATURE_LONGMODE               CPUID_BIT(CPUID_EXT_MODEL_FEAT, 4, 29) //!< Intel® 64 Architecture available if 1, works on AMD too (page 615 of AMD Vol. 3)
// INTEL ONLY
#define IA_FEATURE_VMX                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 5) //!< Virtual Machine Extensions
#define IA_FEATURE_TM2                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 8) //!< Thermal Manager 2
#define IA_FEATURE_PDCM                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 15) //!< PDCM — Perf/Debug Capability MSR
#define IA_FEATURE_PCID                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 17) //!< PCID — Process-context Identifiers
#define IA_FEATURE_X2APIC                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 21) //!< x2APIC 
#define IA_FEATURE_TSCDEADLINE            CPUID_BIT(CPUID_MODEL_FEAT, 2, 24) //!< TSC-Deadline
#define IA_FEATURE_ACPI                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 22) //!< ACPI–Thermal Monitor and Clock Ctr
/* USELESS SHIT that appears on Intel platforms
#define IA_FEATURE_DTES64                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 2)
#define IA_FEATURE_DSCPL                  CPUID_BIT(CPUID_MODEL_FEAT, 2, 4)
#define IA_FEATURE_SMX                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 6)
#define IA_FEATURE_EIST                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 7)
#define IA_FEATURE_CNXTID                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 10)
#define IA_FEATURE_SDBG                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 11)
#define IA_FEATURE_XTPRUPDATECTRL         CPUID_BIT(CPUID_MODEL_FEAT, 2, 14)
#define IA_FEATURE_DCA                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 18)
#define IA_FEATURE_PSN                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 18)
#define IA_FEATURE_DS                     CPUID_BIT(CPUID_MODEL_FEAT, 3, 21)
#define IA_FEATURE_PBE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 31)
*/

// AMD ONLY
//! On AMD platforms, its reserved for hypervisor to indicate guest status (RAV?)
//! On Intel platforms, this should always return 0
#define AMD_FEATURE_HYP                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 31)
#define AMD_FEATURE_TOPO                  CPUID_BIT(CPUID_EXT_MODEL_FEAT, 2, 22)

/* USESLESS SHIT that appears in both platforms
#define x86_FEATURE_PCMULQDQ               CPUID_BIT(CPUID_MODEL_FEAT, 2, 1) //!< PCLMULQDQ (Carryless Multiplication) instruction support
#define x86_FEATURE_FMA                    CPUID_BIT(CPUID_MODEL_FEAT, 2, 12) //!< FMA (Fused Multiply Add) instruction support
#define x86_FEATURE_CMPXCHG16B             CPUID_BIT(CPUID_MODEL_FEAT, 2, 13) //!< CMPXCHG16B instruction support
#define x86_FEATURE_MOVBE                  CPUID_BIT(CPUID_MODEL_FEAT, 2, 22) //!< MOVBE instruction support
#define x86_FEATURE_POPCNT                 CPUID_BIT(CPUID_MODEL_FEAT, 2, 23) //!< POPCNT instruction
#define x86_FEATURE_F16C                   CPUID_BIT(CPUID_MODEL_FEAT, 2, 29) //!< Half-precision convert instruction support
#define x86_FEATURE_CMPXCHG8B              CPUID_BIT(CPUID_MODEL_FEAT, 3, 8)
#define x86_FEATURE_CMOV                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 15)
#define x86_FEATURE_DE                     CPUID_BIT(CPUID_MODEL_FEAT, 3, 2)
#define x86_FEATURE_TSC                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 4)
#define x86_FEATURE_MSR                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 5)
#define x86_FEATURE_MCE                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 7)
#define x86_FEATURE_MTRR                   CPUID_BIT(CPUID_MODEL_FEAT, 3, 12)
#define x86_FEATURE_MCA                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 14)
#define x86_FEATURE_PAT                    CPUID_BIT(CPUID_MODEL_FEAT, 3, 16)
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fetches the CPU vendor string i.e., GenuineIntel
 * @return Pointer to read-only vendor string
 */
const char* cpu_vend_string(void);

#ifdef __cplusplus
}
#endif
