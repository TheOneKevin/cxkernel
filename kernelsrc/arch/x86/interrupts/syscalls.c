/*
 * File:   syscalls.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 07:26:38 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T08:04:56-04:00
*/

#define __MODULE__ "SYSCL"

#include "lib/printk.h"

#include "arch/x86/arch_common.h"
#include "arch/x86/pic.h"
#include "arch/x86/syscalls.h"
#include "arch/x86/interrupts.h"

extern void syscall_asm_stub();

void syscalls_register(void)
{
    OS_PRN("%-66s", "Registering syscalls...");
    fflush(STREAM_OUT);
    idt_set_gate(ARCH_SYSCALL_INT_NO, (unsigned) syscall_asm_stub, 0x08, 0x8E);
    fprintf(STREAM_OUT, "DONE!\n");
}

void syscall_handler(void)
{
    fprintf(STREAM_LOG, "SYSCALL\n");
    PIC_acknowledge();
}
