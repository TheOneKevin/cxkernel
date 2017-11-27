/*
 * Filename: interrupts.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:32:26 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 10-Aug-2017 03:20:36 PM
*/

#include "lib/string.h"
#include "arch/i386/sys/interrupts.h"
#include "arch/i386/sys/pic.h"

irq_t interrupt_handlers[128]; // 16 will do... but no
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

extern void load_idt(uint32_t);

void install_idt()
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
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;

    idt_entries[num].flags = flags /* | 0x60 */;
}

void init_irq()
{
    idt_set_gate(32, (unsigned) irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned) irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned) irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned) irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned) irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned) irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned) irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned) irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned) irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned) irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned) irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned) irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned) irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned) irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned) irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned) irq15, 0x08, 0x8E);
}

void irq_handler(regs_t* regs) // Our interrupt handler called from the assembly file
{
    if (interrupt_handlers[regs -> int_no] != 0)
    {
        irq_t handler = interrupt_handlers[regs -> int_no];
        handler(regs); // Call our handler pointer from the array
    }
    if (regs -> int_no >= 32)
        PIC_sendEOI(regs -> int_no - 32); // Send our EOI, so if IRQ breaks down, we KNOW this isn't the culprit
}

void install_irqhandler(uint8_t irq, irq_t handler)
{
    interrupt_handlers[irq] = handler;
}

void uninstall_irqhandler(uint8_t irq)
{
    interrupt_handlers[irq] = 0;
}
