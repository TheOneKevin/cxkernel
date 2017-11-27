/*
 * Filename: exceptions.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:39:09 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 07:32:41 PM
*/

#include "lib/printk.h"

#include "arch/i386/sys/pic.h"
#include "arch/i386/sys/exceptions.h"
#include "arch/i386/sys/interrupts.h"
#include "arch/i386/sys/stack_trace.h"

// Firstly, let's set up all our error messages
static char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// Here, we add the entries of our ISR to the IDT
void install_isr()
{
    idt_set_gate(0,  (unsigned) isr0, 0x08, 0x8E);
    idt_set_gate(1,  (unsigned) isr1, 0x08, 0x8E);
    idt_set_gate(2,  (unsigned) isr2, 0x08, 0x8E);
    idt_set_gate(3,  (unsigned) isr3, 0x08, 0x8E);
    idt_set_gate(4,  (unsigned) isr4, 0x08, 0x8E);
    idt_set_gate(5,  (unsigned) isr5, 0x08, 0x8E);
    idt_set_gate(6,  (unsigned) isr6, 0x08, 0x8E);
    idt_set_gate(7,  (unsigned) isr7, 0x08, 0x8E);
    idt_set_gate(8,  (unsigned) isr8, 0x08, 0x8E);
    idt_set_gate(9,  (unsigned) isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned) isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned) isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned) isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned) isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned) isr14, 0x08, 0x8E); // Page fault
    idt_set_gate(15, (unsigned) isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned) isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned) isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned) isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned) isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned) isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned) isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned) isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned) isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned) isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned) isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned) isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned) isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned) isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned) isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned) isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned) isr31, 0x08, 0x8E);
}

void isr_handler(regs_t* r) // Our exception handler called from the assembly file
{
    kprintf(" KERNEL EXCEPTION 0x%02X\n %s\n", r -> int_no, exception_messages[r -> int_no]);
    CallStackTrace(r -> ebp, false);
    kprintf("Regdump:\n");
    kprintf("eax: 0x%08X ebx: 0x%08X ecx: 0x%08X edx: 0x%08X err: 0x%08X\n", r -> eax, r -> ebx, r -> ecx, r -> edx, r -> err_code);
    kprintf("edi: 0x%08X esi: 0x%08X esp: 0x%08X ebp: 0x%08X int: 0x%08X\n", r -> edi, r -> esi, r -> esp, r -> ebp, r -> int_no);
    kprintf("gs: 0x%08X fs: 0x%08X es: 0x%08X ds: 0x%08X cs: 0x%08X\n", r -> gs, r -> fs, r -> es, r -> ds, r -> cs);
    kprintf("eflags: 0x%08X eip: 0x%08X useresp: 0x%08X ss: 0x%08X\n", r -> eflags, r -> eip, r -> useresp, r -> ss);
    for(;;);
    PIC_acknowledge();
}
