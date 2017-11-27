/*
 * File:   syscalls.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 07:26:38 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:45:53-05:00
*/

#include "lib/printk.h"

#include "arch/x86/arch_common.h"
#include "arch/x86/pic.h"
#include "arch/x86/syscalls.h"
#include "arch/x86/interrupts.h"

extern void syscall_asm_stub();

void
syscalls_register(void)
{
    fprintf(STREAM_OUT, "Registering syscalls... ");
    fflush(STREAM_OUT);
    idt_set_gate(ARCH_SYSCALL_INT_NO, (unsigned) syscall_asm_stub, 0x08, 0x8E);
    fprintf(STREAM_OUT, "Done!\n");
}

void syscall_handler(void)
{
    fprintf(STREAM_LOG, "SYSCALL\n");
    PIC_acknowledge();
}
