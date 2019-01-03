/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   gdt.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 02 2019, 4:14 PM
 */

#include "arch/x86/32/gdt.h"
#include "include/global.h"

extern "C" {

static gdt_entry_t gdt_entries[6]; // null, code, data, user code, user data, tss
static gdt_ptr_t gdt_ptr;
extern "C" void load_gdt(uint32_t);
static void set_gate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[idx].base_low = (base & 0xFFFF);
    gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt_entries[idx].base_high = (base >> 24) & 0xFF;
    gdt_entries[idx].limit_low = (limit & 0xFFFF);
    gdt_entries[idx].granularity = (limit >> 16) & 0x0F;
    gdt_entries[idx].granularity |= gran & 0xF0;
    gdt_entries[idx].access = access;
}
void initgdt32(void)
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;
    set_gate(0, 0, 0, 0, 0);                // Null segment
    set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    load_gdt((uint32_t) &gdt_ptr);
}

}
