/*
 * File:   exceptions.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:39:09 PM
 *
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-05-27T09:48:49-04:00
*/

#include "lib/printk.h"
#include "arch/x86/llio.h"

#include "arch/x86/pic.h"
#include "arch/x86/exceptions.h"
#include "arch/x86/interrupts.h"
#include "arch/x86/stack_trace.h"

// Firstly, let's set up all our error messages
static char* exception_messages[] =
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

static char* page_fault_messages[] =
{
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault"
};

// Here, we add the entries of our ISR to the IDT
void install_isr(void)
{
    idt_set_gate(0,  (unsigned int) isr0, 0x08, 0x8E);
    idt_set_gate(1,  (unsigned int) isr1, 0x08, 0x8E);
    idt_set_gate(2,  (unsigned int) isr2, 0x08, 0x8E);
    idt_set_gate(3,  (unsigned int) isr3, 0x08, 0x8E);
    idt_set_gate(4,  (unsigned int) isr4, 0x08, 0x8E);
    idt_set_gate(5,  (unsigned int) isr5, 0x08, 0x8E);
    idt_set_gate(6,  (unsigned int) isr6, 0x08, 0x8E);
    idt_set_gate(7,  (unsigned int) isr7, 0x08, 0x8E);
    idt_set_gate(8,  (unsigned int) isr8, 0x08, 0x8E);
    idt_set_gate(9,  (unsigned int) isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned int) isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int) isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int) isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int) isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int) isr14, 0x08, 0x8E); // Page fault
    idt_set_gate(15, (unsigned int) isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int) isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int) isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int) isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int) isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int) isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int) isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int) isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int) isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int) isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int) isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int) isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int) isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int) isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int) isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int) isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int) isr31, 0x08, 0x8E);
}

void isr_handler(regs_t* r) // Our exception handler called from the assembly file
{
    fprintf(STREAM_ERR, " === KERNEL EXCEPTION 0x%02X ===\n %s\n", r -> int_no, exception_messages[r -> int_no]);

    if(r -> int_no == 14) // Page fault
    {
        // The error code gives us details of what happened.
        fprintf(STREAM_ERR, " %s\n", page_fault_messages[r -> err_code]);
        fprintf(STREAM_ERR, " at: 0x%08X\n", read_cr2());
    }

    CallStackTrace(r -> ebp, false);

    fprintf(STREAM_ERR, " === Regdump ===\n");
    fprintf(STREAM_ERR, "eax: 0x%08X ebx: 0x%08X ecx: 0x%08X edx: 0x%08X err: 0x%08X\n", r -> eax, r -> ebx, r -> ecx, r -> edx, r -> err_code);
    fprintf(STREAM_ERR, "edi: 0x%08X esi: 0x%08X esp: 0x%08X ebp: 0x%08X int: 0x%08X\n", r -> edi, r -> esi, r -> esp, r -> ebp, r -> int_no);
    fprintf(STREAM_ERR, "gs:  0x%08X fs:  0x%08X es:  0x%08X ds:  0x%08X cs:  0x%08X\n", r -> gs, r -> fs, r -> es, r -> ds, r -> cs);
    fprintf(STREAM_ERR, "cr0: 0x%08X cr2: 0x%08X cr3: 0x%08X cr4: 0x%08X\n\n", read_cr0(), read_cr2(), read_cr3(), read_cr4());
    fprintf(STREAM_ERR, "eflags: 0x%08X eip: 0x%08X useresp: 0x%08X ss: 0x%08X\n", r -> eflags, r -> eip, r -> useresp, r -> ss);
    
    for(;;);
    PIC_acknowledge();
}
