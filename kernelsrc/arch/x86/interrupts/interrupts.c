/*
 * File:   interrupts.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:32:26 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:05:07-05:00
*/

#include "lib/string.h"
#include "arch/x86/interrupts.h"
#include "arch/x86/pic.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

irq_t interrupt_handlers[128]; // 16 will do... but no

void init_irq()
{
    idt_set_gate(32, (unsigned int) irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned int) irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned int) irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned int) irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned int) irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned int) irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned int) irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned int) irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned int) irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned int) irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned int) irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int) irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int) irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int) irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int) irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int) irq15, 0x08, 0x8E);
}

void irq_handler(regs_t* regs) // Our interrupt handler called from the assembly file
{
    // TODO: Improve somehow
    if (interrupt_handlers[regs -> int_no] != 0)
    {
        irq_t handler = interrupt_handlers[regs -> int_no];
        handler(regs); // Call our handler pointer from the array
    }
    if (regs -> int_no >= 32)
        PIC_sendEOI(regs -> int_no - 32); // Send our EOI, so if IRQ breaks down, we KNOW this isn't the culprit
}

void install_irqhandler(int irq, irq_t handler)
{
    interrupt_handlers[irq] = handler;
}

void uninstall_irqhandler(int irq)
{
    interrupt_handlers[irq] = 0;
}
