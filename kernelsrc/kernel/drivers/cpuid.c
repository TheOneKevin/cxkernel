/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "drivers/cpuid.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"

enum
{
    CPUID_FEAT_ECX_SSE3         = 1 << 0, 
    CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
    CPUID_FEAT_ECX_DTES64       = 1 << 2,
    CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
    CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
    CPUID_FEAT_ECX_VMX          = 1 << 5,  
    CPUID_FEAT_ECX_SMX          = 1 << 6,  
    CPUID_FEAT_ECX_EST          = 1 << 7,  
    CPUID_FEAT_ECX_TM2          = 1 << 8,  
    CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
    CPUID_FEAT_ECX_CID          = 1 << 10,
    CPUID_FEAT_ECX_FMA          = 1 << 12,
    CPUID_FEAT_ECX_CX16         = 1 << 13, 
    CPUID_FEAT_ECX_ETPRD        = 1 << 14, 
    CPUID_FEAT_ECX_PDCM         = 1 << 15, 
    CPUID_FEAT_ECX_DCA          = 1 << 18, 
    CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
    CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
    CPUID_FEAT_ECX_x2APIC       = 1 << 21, 
    CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
    CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
    CPUID_FEAT_ECX_AES          = 1 << 25, 
    CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
    CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
    CPUID_FEAT_ECX_AVX          = 1 << 28,
    CPUID_FEAT_ECX_F16C         = 1 << 29,
    CPUID_FEAT_ECX_RDRAND       = 1 << 30,
 
    CPUID_FEAT_EDX_FPU          = 1 << 0,  
    CPUID_FEAT_EDX_VME          = 1 << 1,  
    CPUID_FEAT_EDX_DE           = 1 << 2,  
    CPUID_FEAT_EDX_PSE          = 1 << 3,  
    CPUID_FEAT_EDX_TSC          = 1 << 4,  
    CPUID_FEAT_EDX_MSR          = 1 << 5,  
    CPUID_FEAT_EDX_PAE          = 1 << 6,  
    CPUID_FEAT_EDX_MCE          = 1 << 7,  
    CPUID_FEAT_EDX_CX8          = 1 << 8,  
    CPUID_FEAT_EDX_APIC         = 1 << 9,  
    CPUID_FEAT_EDX_SEP          = 1 << 11, 
    CPUID_FEAT_EDX_MTRR         = 1 << 12, 
    CPUID_FEAT_EDX_PGE          = 1 << 13, 
    CPUID_FEAT_EDX_MCA          = 1 << 14, 
    CPUID_FEAT_EDX_CMOV         = 1 << 15, 
    CPUID_FEAT_EDX_PAT          = 1 << 16, 
    CPUID_FEAT_EDX_PSE36        = 1 << 17, 
    CPUID_FEAT_EDX_PSN          = 1 << 18, 
    CPUID_FEAT_EDX_CLF          = 1 << 19, 
    CPUID_FEAT_EDX_DTES         = 1 << 21, 
    CPUID_FEAT_EDX_ACPI         = 1 << 22, 
    CPUID_FEAT_EDX_MMX          = 1 << 23, 
    CPUID_FEAT_EDX_FXSR         = 1 << 24, 
    CPUID_FEAT_EDX_SSE          = 1 << 25, 
    CPUID_FEAT_EDX_SSE2         = 1 << 26, 
    CPUID_FEAT_EDX_SS           = 1 << 27, 
    CPUID_FEAT_EDX_HTT          = 1 << 28, 
    CPUID_FEAT_EDX_TM1          = 1 << 29, 
    CPUID_FEAT_EDX_IA64         = 1 << 30,
    CPUID_FEAT_EDX_PBE          = 1 << 31
};

enum cpuid_requests
{
  CPUID_GETVENDORSTRING,
  CPUID_GETFEATURES,
  CPUID_GETTLB,
  CPUID_GETSERIAL,
 
  CPUID_INTELEXTENDED=0x80000000,
  CPUID_INTELFEATURES,
  CPUID_INTELBRANDSTRING,
  CPUID_INTELBRANDSTRINGMORE,
  CPUID_INTELBRANDSTRINGEND,
};

/* Vendor-strings. */
char* CPUID_VENDOR_OLDAMD       = "AMDisbetter!"; //early engineering samples of AMD K5 processor
char* CPUID_VENDOR_AMD          = "AuthenticAMD";
char* CPUID_VENDOR_INTEL        = "GenuineIntel";
#define CPUID_VENDOR_VIA          "CentaurHauls"
#define CPUID_VENDOR_OLDTRANSMETA "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA    "GenuineTMx86"
#define CPUID_VENDOR_CYRIX        "CyrixInstead"
#define CPUID_VENDOR_CENTAUR      "CentaurHauls"
#define CPUID_VENDOR_NEXGEN       "NexGenDriven"
#define CPUID_VENDOR_UMC          "UMC UMC UMC "
#define CPUID_VENDOR_SIS          "SiS SiS SiS "
#define CPUID_VENDOR_NSC          "Geode by NSC"
#define CPUID_VENDOR_RISE         "RiseRiseRise"

uint32_t isIntel = false;

bool _FPU = false;
bool _VME = false;
bool _PSE = false;
bool _PAE = false;
bool _APIC = false;
bool _ACPI = false;
bool _MTRR = false;
bool _HTT = false;
bool _TM1 = false;

bool _TSC = false;
bool _MSR = false;
bool _SSE = false;
bool _SSE2 = false;
bool _SSE3 = false;
bool _SSSE3 = false;
bool _SSE4_1 = false;
bool _SSE4_2 = false;
bool _AVX = false;
bool _F16C = false;
bool _RDRAND = false;

bool _IA64 = false;

uint32_t _CORES = 0;

static inline void cpuid(uint32_t code, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (code));
}

void cpu_detect()
{
    uint32_t a, b, c, d;
    uint32_t largestFunc;
    char vendor[12];
    cpuid(CPUID_GETVENDORSTRING, &largestFunc, (uint32_t*) (vendor + 0), (uint32_t*) (vendor + 8), (uint32_t*) (vendor + 4));
    bprintinfo(); kprintf("CPU Vendor: %s\n", vendor);
    if(vendor == CPUID_VENDOR_AMD)
        isIntel = 2;
    else if(vendor == CPUID_VENDOR_INTEL)
        isIntel = 1;
    
    if(largestFunc >= CPUID_GETFEATURES)
    {
        cpuid(CPUID_GETFEATURES, &a, &b, &c, &d);
        
        bprintinfo(); kprintf("Feature Set: ");
        if(d & CPUID_FEAT_EDX_FPU) { kprintf("FPU "); _FPU = true; }
        if(d & CPUID_FEAT_EDX_VME) { kprintf("VME "); _VME = true; }
        if(d & CPUID_FEAT_EDX_PSE) { kprintf("PSE "); _PSE = true; }
        if(d & CPUID_FEAT_EDX_PAE) { kprintf("PAE "); _PAE = true; }
        if(d & CPUID_FEAT_EDX_APIC){ kprintf("APIC "); _APIC = true; }
        if(d & CPUID_FEAT_EDX_ACPI){ kprintf("ACPI "); _ACPI = true; }
        if(d & CPUID_FEAT_EDX_MTRR){ kprintf("MTRR "); _MTRR = true; }
        if(d & CPUID_FEAT_EDX_HTT) { kprintf("HTT "); _HTT = true; }
        if(d & CPUID_FEAT_EDX_TM1) { kprintf("TM"); _TM1 = true; }
        
        console_putc('\n'); bprintinfo(); kprintf("Instruction Set: ");
        if(d & CPUID_FEAT_EDX_TSC)    { kprintf("TSC "); _TSC = true; }
        if(d & CPUID_FEAT_EDX_MSR)    { kprintf("MSR "); _MSR = true; }
        if(d & CPUID_FEAT_EDX_SSE)    { kprintf("SSE "); _SSE = true; }
        if(d & CPUID_FEAT_EDX_SSE2)   { kprintf("SSE2 "); _SSE2 = true; }
        
        if(c & CPUID_FEAT_ECX_SSE3)   { kprintf("SSE3 "); _SSE3 = true; }
        if(c & CPUID_FEAT_ECX_SSSE3)  { kprintf("SSSE3 "); _SSSE3 = true; }
        if(c & CPUID_FEAT_ECX_SSE4_1) { kprintf("SSE4.1 "); _SSE4_1 = true; }
        if(c & CPUID_FEAT_ECX_SSE4_2) { kprintf("SSE4.2 "); _SSE4_2 = true; }
        if(c & CPUID_FEAT_ECX_AVX)    { kprintf("AVX "); _AVX = true; }
        if(c & CPUID_FEAT_ECX_F16C)   { kprintf("F16C "); _F16C = true; }
        if(c & CPUID_FEAT_ECX_RDRAND) { kprintf("RDRAND "); _RDRAND = true; }
        
        if(_HTT)
        {
            //Get core count from bits 16 to 23
            _CORES = (b >> 16) & ~(~0 << (23 - 16 + 1));
        }
        
        console_putc('\n');
    }
    
    uint32_t largestFuncX;
    cpuid(CPUID_INTELEXTENDED, &largestFuncX, &b, &c, &d);
    
    if(largestFuncX >= CPUID_INTELEXTENDED)
    {
        cpuid(CPUID_INTELFEATURES, &a, &b, &c, &d);
        if(d & CPUID_FEAT_EDX_IA64)
        {
            bprintinfo(); kprintf("64-bit Architecture");
            _IA64 = true;
        }
    }
    
    if(largestFuncX >= CPUID_INTELBRANDSTRINGEND)
    {
        char name[48];
        cpuid(CPUID_INTELBRANDSTRING, (uint32_t *)(name), (uint32_t *)(name + 4), (uint32_t *)(name + 8), (uint32_t *)(name + 12));
        cpuid(CPUID_INTELBRANDSTRINGMORE, (uint32_t *)(name + 16), (uint32_t *)(name + 20), (uint32_t *)(name + 24), (uint32_t *)(name + 28));
        cpuid(CPUID_INTELBRANDSTRINGEND, (uint32_t *)(name + 32), (uint32_t *)(name + 36), (uint32_t *)(name + 40), (uint32_t *)(name + 44));
        const char *p = name;
        while(*p == ' ')
            ++p; //We don't want spaces in our name
        bprintinfo(); kprintf("CPU Name: %s\n", p);
    }
}