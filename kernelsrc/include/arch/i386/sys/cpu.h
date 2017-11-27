/*
 * File:   cpu.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 09-Aug-2017 02:14:08 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 09-Aug-2017 02:22:57 PM
*/

#pragma once

#include "system.h"

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
#define CPUID_VENDOR_PARALLELS  " lrpepyh vr" // haha should be "prl hyperv" but some idiot messed up
#define CPUID_VENDOR_VMWARE     "VMwareVMWare"
#define CPUID_VENDOR_XENVMM     "XenVMMXenVMM"

#ifdef __cplusplus
extern "C" {
#endif

// C code goes here

#ifdef __cplusplus
}
#endif
