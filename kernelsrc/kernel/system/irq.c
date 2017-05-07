/*
 * The interrupt handlers code in C
 */

#include "system/irq.h"
#include "system/pic.h"
#include "display/tdisplay.h"
#include "system/kprintf.h"
#include "arch/idt.h"
#include "arch/exceptions.h"

irq_t interrupt_handlers[256]; // 16 will do... but no

void register_irq()
{
    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
    bprintok(); console_writeline("Registered IRQ handlers");
}

void irq_handler(regs_t *regs) // We need to call regs as a reference or it won't work :)
{
    // Uncomment for debugging purposes
    /*console_write_dec(regs->int_no);
    console_putc(' ');*/
    if(interrupt_handlers[regs->int_no] != 0)
    {
        irq_t handler = interrupt_handlers[regs->int_no];
        handler(regs); //Call our handler pointer from the array
    }
    if(regs->int_no >= 32)
        PIC_sendEOI(regs->int_no - 32); // Send our EOI, so if IRQ breaks down, we KNOW this isn't the culprit
}

void install_handler(uint8_t irq, irq_t handler)
{
    interrupt_handlers[irq] = handler; // Register the handler pointer
    //bprintok(); kprintf("Installed handler: %u\n", (uint32_t) irq);
}

void uninstall_handler(uint8_t irq)
{
    interrupt_handlers[irq] = 0; // Clear the handler pointer
    //bprintok(); kprintf("Uninstalled handler: %u\n", (uint32_t) irq);
}
