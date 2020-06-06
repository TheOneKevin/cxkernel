/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   cpu.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on August 09 2018, 2:10 PM
 */

#include <string.h>

#include "arch/x86/cpu.h"
#include "arch/x86/llio.h"

struct cpuid_leaf _cpuid[MAX_SUPPORTED_CPUID + 1];
struct cpuid_leaf _cpuid_ext[MAX_SUPPORTED_CPUID_EXT - (uint32_t) CPUID_EXT_BASE + 1];
uint32_t max_cpuid = 0;
uint32_t max_ext_cpuid = 0;

enum cpu_vendor_list x86_vendor;

static void x86_cpuid_init(void)
{
    cpuid(CPUID_BASE, &_cpuid[0].a, &_cpuid[0].b, &_cpuid[0].c, &_cpuid[0].d);
    max_cpuid = _cpuid[0].a;
    if(max_cpuid > MAX_SUPPORTED_CPUID)
        max_cpuid = MAX_SUPPORTED_CPUID;
    for(uint32_t i = 1; i <= max_cpuid; i++)
        cpuidc(i, 0, &_cpuid[i].a, &_cpuid[i].b, &_cpuid[i].c, &_cpuid[i].d);
    
    union
    {
        uint32_t id[3];
        char vendor[12];
    } v { };

    v.id[0] = _cpuid[0].b;
    v.id[1] = _cpuid[1].c;
    v.id[2] = _cpuid[2].d;

    if(!memcmp(v.vendor, CPUID_VENDOR_INTEL, 12))
        x86_vendor = x86_VENDOR_INTEL;
    else if(!memcmp(v.vendor, CPUID_VENDOR_AMD, 12))
        x86_vendor = x86_VENDOR_AMD;
    else
        x86_vendor = x86_VENDOR_UNKNOWN;
    
    cpuid(CPUID_EXT_BASE, &_cpuid_ext[0].a, &_cpuid_ext[0].b, &_cpuid_ext[0].c, &_cpuid_ext[0].d);
    max_ext_cpuid = _cpuid_ext[0].a;
    if(max_ext_cpuid > MAX_SUPPORTED_CPUID_EXT)
        max_ext_cpuid = MAX_SUPPORTED_CPUID_EXT;
    for(uint32_t i = 1; i - 1 < max_ext_cpuid - CPUID_EXT_BASE; i++)
        cpuidc(i + CPUID_EXT_BASE, 0, &_cpuid_ext[i].a, &_cpuid_ext[i].b, &_cpuid_ext[i].c, &_cpuid_ext[i].d);
} EXPORT_CTOR(x86_cpuid_init);
