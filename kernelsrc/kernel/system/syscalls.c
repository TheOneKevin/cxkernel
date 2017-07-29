#include "system.h"
#include "arch/x86/idt.h"

extern syscallf;

void initSyscalls()
{
    idt_set_gate(0x80, (unsigned)syscallf, 0x08, 0x8E);
}

void syscall_handler()
{
    kprintf("Hello");
    for(;;);
}
