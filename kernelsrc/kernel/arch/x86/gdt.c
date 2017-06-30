#include "arch/x86/gdt.h"
#include "display/tdisplay.h"

void install_gdt()
{
    // Set up our GDT pointer
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;
    // Set up our GDT entries
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    //gdt_set_gate(5, 0, 0xFFFFFFFF, 0x9A, 0x0F); // 16 bit code segment
    //gdt_set_gate(6, 0, 0xFFFFFFFF, 0x92, 0x0F); // 16 bit data segment

    // Call our external assembly function to load the GDT
    load_gdt((uint32_t)&gdt_ptr);
    bprintok(); console_writeline("GDT initialized!");
}
// Let's add a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}
