/*
 * Filename: syscalls.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 07:26:38 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 07:37:52 PM
*/

#include "lib/printk.h"

#include "arch/i386/common.h"
#include "arch/i386/sys/pic.h"
#include "arch/i386/sys/syscalls.h"
#include "arch/i386/sys/interrupts.h"

extern void syscall_asm_stub();

void syscalls_register()
{
    idt_set_gate(SYSCALL_INT_NO, (unsigned) syscall_asm_stub, 0x08, 0x8E);
}

void syscall_handler()
{
    kprintf("SYSCALL\n");
    PIC_acknowledge();
}
