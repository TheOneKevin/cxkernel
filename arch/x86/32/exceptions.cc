/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   exceptions.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 04 2019, 11:57 PM
 */

#define __MODULE__ "INTR"

#include <string.h>
#include <stdio.h>
#include "arch/x86/llio.h"
#include "arch/x86/arch_utils.h"
#include "platform/pc/pic.h"
#include "platform.h"
#include "arch/x86/32/idt.h"
#include "arch/x86/strace.h"

// Firstly, let's set up all our error messages
static const char* exception_messages[] = {
    "#DE Division By Zero",
    "#DB Debug",
    "Non Maskable Interrupt",
    "#BP Breakpoint",
    "#OF Into Detected Overflow",
    "#BR Out of Bounds",
    "#UD Invalid Opcode",
    "#NM No Coprocessor",
    "#DF Double Fault",
    "Coprocessor Segment Overrun",
    "#TS Bad TSS",
    "#NP Segment Not Present",
    "#SS Stack-Segment Fault",
    "#GP General Protection Fault",
    "#PF Page Fault",
    "Unknown Interrupt",
    "#MF Coprocessor Fault",
    "#AC Alignment Check",
    "#MC Machine Check",
    "#XF SIMD Floating-Point Exception",
    "#VE Virtualization Exception",
    "Reserved",
    "#SX Security Exception",
    "Reserved",
    "Triple Fault",
    "#FERR Legacy",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static const char* page_fault_messages[] = {
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault"
};

void exception_handler(regs_t* r) // Our exception handler called from the assembly file
{
    fprintf(STREAM_ERR, " === KERNEL UNHANDLED EXCEPTION 0x%02X ===\n %s\n", r->int_no, exception_messages[r->int_no]);

    if(r->int_no == 14) // Page fault
    {
        // The error code gives us details of what happened.
        fprintf(STREAM_ERR, " %s\n", page_fault_messages[r->err_code]);
        fprintf(STREAM_ERR, " at: 0x%08X\n", read_cr2());
    }

    CallStackTrace(r->ebp, false);

    fprintf(STREAM_ERR, " === Dump ===\n");
    fprintf(STREAM_ERR, "eax: 0x%08X ebx: 0x%08X ecx: 0x%08X edx: 0x%08X err: 0x%08X\n", r->eax, r->ebx, r->ecx, r->edx, r->err_code);
    fprintf(STREAM_ERR, "edi: 0x%08X esi: 0x%08X esp: 0x%08X ebp: 0x%08X int: 0x%08X\n", r->edi, r->esi, r->esp, r->ebp, r->int_no);
    fprintf(STREAM_ERR, "gs:  0x%08X fs:  0x%08X es:  0x%08X ds:  0x%08X cs:  0x%08X\n", r->gs, r->fs, r->es, r->ds, r->cs);
    fprintf(STREAM_ERR, "cr0: 0x%08X cr2: 0x%08X cr3: 0x%08X cr4: 0x%08X\n\n", read_cr0(), read_cr2(), read_cr3(), read_cr4());
    fprintf(STREAM_ERR, "eflags: 0x%08X eip: 0x%08X ss: 0x%08X\n", r->eflags, r->eip, r->ss);

    for(;;) HALT;
}

extern "C" void isr_handler(regs_t* r)
{
    if(platform::get_irq().get_handler(r->int_no) != 0)
        platform::get_irq().get_handler(r->int_no)(r);
    else
        OS_ERR("Unhandled exception 0x%X!\n", r->int_no);
    pc::pic::send_eoi(r->int_no);
}
