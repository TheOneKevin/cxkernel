/*
 * File:   gdt.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 30-Jul-2017 04:28:56 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-11-26T12:40:22-05:00
*/

#include "lib/string.h"
#include "arch/x86/gdt.h"

gdt_entry_t gdt_entries[6]; // null, code, data, user code, user data, tss
gdt_ptr_t gdt_ptr;
tss_t tss_entry;

extern void load_gdt(uint32_t);
extern void load_tss(void);

void init_gdt(void)
{
    // Set up our GDT pointer
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;
    // Calculate TSS limit and base
    uint32_t tss_base = (uint32_t) &tss_entry;
    uint32_t tss_limit = tss_base + sizeof(tss_t);

    // Set up our GDT entries
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    gdt_set_gate(5, tss_base, tss_limit, 0xE9, 0x00); // TSS

    // gdt_set_gate(5, 0, 0xFFFFFFFF, 0x9A, 0x0F); // 16 bit code segment
    // gdt_set_gate(6, 0, 0xFFFFFFFF, 0x92, 0x0F); // 16 bit data segment

    // Clear and initialize our TSS
    memset(&tss_entry, 0x0, sizeof(tss_t));
    tss_entry.SS0 = 0x10;
    tss_entry.ESP0 = 0x0;
    tss_entry.CS = 0x0B;
    tss_entry.SS =
    tss_entry.ES =
    tss_entry.FS =
    tss_entry.GS = 0x13;
    tss_entry.IO_MAP_BASE = sizeof(tss_entry);

    // Call our external assembly function to load the GDT
    load_gdt((uint32_t) &gdt_ptr);
    load_tss();
}

// Let's add a GDT entry
void gdt_set_gate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[idx].base_low = (base & 0xFFFF);
    gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt_entries[idx].base_high = (base >> 24) & 0xFF;

    gdt_entries[idx].limit_low = (limit & 0xFFFF);
    gdt_entries[idx].granularity = (limit >> 16) & 0x0F;

    gdt_entries[idx].granularity |= gran & 0xF0;
    gdt_entries[idx].access = access;
}

void set_tss_stack(uint32_t stack)
{
    tss_entry.ESP0 = stack;
}
