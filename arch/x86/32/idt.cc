/*
 * @file   idt.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on 2017-10-02T16:35:17-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-11-26T12:40:25-05:00
*/

#include <string.h>
#include "arch/x86/interrupts.h"

namespace x86_32::idt {

static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;
extern "C" void load_idt(uint32_t);

void init(void)
{
    // Set up our pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t) &idt_entries;
    // Clear our interrupts array
    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);
    // Call our external assembly method
    load_idt((uint32_t) &idt_ptr);
}

// Let's add an IDT entry!
void set_gate(int idx, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[idx].base_lo = base & 0xFFFF;
    idt_entries[idx].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[idx].sel = sel;
    idt_entries[idx].always0 = 0;

    idt_entries[idx].flags = flags /* | 0x60 */;
}

}
