/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arch/idt.h"
#include "display/tdisplay.h"

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

void install_idt()
{
    // Set up our pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t) &idt_entries;
    // Clear our interrupts array
    memset(&idt_entries, 0 , sizeof(idt_entry_t) * 256);
    // Call our external assembly method
    load_idt((uint32_t)&idt_ptr);
    bprintok(); console_writeline("IDT initialized!");
}

//Let's add an IDT entry!
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    
    idt_entries[num].flags   = flags /* | 0x60 */;
}