/*
 * File:   gdt.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:28:56 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:40:22-05:00
*/

#include "arch/x86/gdt.h"

gdt_entry_t gdt_entries[5]; // null, code, data, user code, user data
gdt_ptr_t gdt_ptr;
extern void load_gdt(uint32_t);

void install_gdt(void)
{
    // Set up our GDT pointer
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;
    // Set up our GDT entries
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    // gdt_set_gate(5, 0, 0xFFFFFFFF, 0x9A, 0x0F); // 16 bit code segment
    // gdt_set_gate(6, 0, 0xFFFFFFFF, 0x92, 0x0F); // 16 bit data segment

    // Call our external assembly function to load the GDT
    load_gdt((uint32_t) &gdt_ptr);
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
