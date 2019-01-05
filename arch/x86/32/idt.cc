/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   idt.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 10, 2017, 4:35 PM
 */

#define __MODULE__ "INTR"

#include <string.h>
#include <stdio.h>
#include "platform/pc/pic.h"
#include "platform.h"
#include "arch/x86/32/idt.h"

namespace x86_32::idt
{
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
        idt_entries[idx].base_lo = (uint16_t) base;
        idt_entries[idx].base_hi = (uint16_t)(base >> 16);
        idt_entries[idx].sel = sel;
        idt_entries[idx].always0 = 0;
        idt_entries[idx].flags = flags /* | 0x60 */;
    }

    uint32_t get_gate(int idx)
    {
        return (uint32_t) (idt_entries[idx].base_hi << 16) | idt_entries[idx].base_lo;
    }
} // namespace x86_32::idt

extern "C" void isr_handler(regs_t* r)
{
    if(platform::get_irq().get_handler(r->int_no) != 0)
        platform::get_irq().get_handler(r->int_no)(r);
    else
        OS_ERR("Unhandled exception 0x%X!\n", r->int_no);
    pc::pic::send_eoi(r->int_no);
}
