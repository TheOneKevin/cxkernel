/* Declare constants for the multiboot header */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/* Map our kernel to the higher half */
#include "arch/x86/arch_common.h"
#include "arch/x86/interface/arch_interface.h"
#define KRNL_PAGE_NUMBER (ARCH_VIRT_BASE >> 22)

/*
 * Declare a multiboot header that marks the program as a kernel. These are magic
 * values that are documented in the multiboot standard. The bootloader will
 * search for this signature in the first 8 KiB of the kernel file, aligned at a
 * 32-bit boundary. The signature is in its own section so the header can be
 * forced to be within the first 8 KiB of the kernel file.
*/

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
 * The multiboot standard does not define the value of the stack pointer register
 * (esp) and it is up to the kernel to provide a stack. This allocates room for a
 * small stack by creating a symbol at the bottom of it, then allocating 16384
 * bytes for it, and finally creating a symbol at the top. The stack grows
 * downwards on x86. The stack is in its own section so it can be marked nobits,
 * which means the kernel file is smaller because it does not contain an
 * uninitialized stack. The stack on x86 must be 16-byte aligned according to the
 * System V ABI standard and de-facto extensions. The compiler will assume the
 * stack is properly aligned and failure to align the stack will result in
 * undefined behavior.
*/

.section .bootloader_stack, "aw", @nobits
.align ARCH_PAGE_SIZE
.global stack_top
.global stack_bottom
stack_bottom:
.skip ARCH_PAGE_SIZE # 4 KB
stack_top:

/* Paging tables and directories for higher half kernel */
/* Statically allocated for temporary usage */
.section .bss, "aw", @nobits
_perserve_magic:
    .skip 4
_perserve_struct:
    .skip 4

.align ARCH_PAGE_SIZE
_kernel_dir1:
    .skip ARCH_PAGE_SIZE
_kernel_table1: /* For the kernel */
    .skip ARCH_PAGE_SIZE
_kernel_table2: /* For the stack */
    .skip ARCH_PAGE_SIZE
_kernel_table3: /* For ACPI */
    .skip ARCH_PAGE_SIZE
_kernel_table4: /* NULL */
    .skip ARCH_PAGE_SIZE
_kernel_table5: /* NULL */
    .skip ARCH_PAGE_SIZE
_kernel_table6: /* NULL */
    .skip ARCH_PAGE_SIZE
_kernel_table7: /* NULL */
    .skip ARCH_PAGE_SIZE

_kernel_dir2: /* For PAE 1 */
    .skip ARCH_PAGE_SIZE
_kernel_dir3: /* For PAE 1 */
    .skip ARCH_PAGE_SIZE
_kernel_dir4: /* For PAE 1 */
    .skip ARCH_PAGE_SIZE

.global _kernel_dir1
.global _kernel_dir2
.global _kernel_dir3
.global _kernel_dir4
.global _kernel_table1
.global _kernel_table2
.global _kernel_table3
.global _kernel_table4
.global _kernel_table5
.global _kernel_table6
.global _kernel_table7

/*
 * The linker script specifies _start as the entry point to the kernel and the
 * bootloader will jump to this position once the kernel has been loaded. It
 * doesn't make sense to return from this function as the bootloader is gone.
*/

.section .text
.global _start
.type _start, @function

.extern 

_start:
    /* Oh how I hate GAS */
    cli

    /* Save the GRUB registers */
    movl %eax, _perserve_magic - ARCH_VIRT_BASE
    movl %ebx, _perserve_struct - ARCH_VIRT_BASE

    movl $(_kernel_table1 - ARCH_VIRT_BASE), %edi
    movl $0, %esi
    movl $1024, %ecx
1:
    movl %esi, %edx             /* esi is the physical address */
    orl  $0x3, %edx             /* Mark as present and writable */
    movl %edx, (%edi)           /* Write to page table */
    addl $ARCH_PAGE_SIZE, %esi  /* Increment our pointer to the current mapped address */
    addl $4, %edi               /* Increment our pointer to the current entry in the table */
    loop 1b
3:
    movl $(_kernel_table2 - ARCH_VIRT_BASE), %edi
    movl $(stack_bottom - ARCH_VIRT_BASE), %edx
    orl $0x3, %edx
    movl $(_kernel_table2 - ARCH_VIRT_BASE + 4 * 1023), %edi
    movl %edx, (%edi)
    /* Identity map the kernel table 1 */
    movl $(_kernel_table1 - ARCH_VIRT_BASE + 0x3), _kernel_dir1 - ARCH_VIRT_BASE + 0
    /* Map the kernel to virt base */
    movl $(_kernel_table1 - ARCH_VIRT_BASE + 0x3), _kernel_dir1 - ARCH_VIRT_BASE + KRNL_PAGE_NUMBER * 4
    /* Map the stack to 0xFF800000 */
    movl $(_kernel_table2 - ARCH_VIRT_BASE + 0x3), _kernel_dir1 - ARCH_VIRT_BASE + 0xFF4
    /* Move the address of the PD to CR3 */
    movl $(_kernel_dir1 - ARCH_VIRT_BASE), %ecx
    movl %ecx, %cr3
    movl %cr0, %ecx
    orl  $0x80010001, %ecx
    movl %ecx, %cr0 /* Enable paging! */

    lea 4f, %ecx
    jmp *%ecx
4:
    /*
     * The bootloader has loaded us into 32-bit protected mode on a x86
     * machine. Interrupts are disabled. Paging is disabled. The processor
     * state is as defined in the multiboot standard. The kernel has full
     * control of the CPU. The kernel can only make use of hardware features
     * and any code it provides as part of itself. There's no printf
     * function, unless the kernel provides its own <stdio.h> header and a
     * printf implementation. There are no security restrictions, no
     * safeguards, no debugging mechanisms, only what the kernel provides
     * itself. It has absolute and complete power over the machine.
     *
     * To set up a stack, we set the esp register to point to the top of our
     * stack (as it grows downwards on x86 systems). This is necessarily done
     * in assembly as languages such as C cannot function without a stack.
    */

    mov $ARCH_STACK_TOP, %esp
    call _init

    /*
     * This is a good place to initialize crucial processor state before the
     * high-level kernel is entered. It's best to minimize the early
     * environment where crucial features are offline. Note that the
     * processor is not fully initialized yet: Features such as floating
     * point instructions and instruction set extensions are not initialized
     * yet. The GDT should be loaded here. Paging should be enabled here.
     * C++ features such as global constructors and exceptions will require
     * runtime support to work as well.
     *
     * Enter the high-level kernel. The ABI requires the stack is 16-byte
     * aligned at the time of the call instruction (which afterwards pushes
     * the return pointer of size 4 bytes). The stack was originally 16-byte
     * aligned above and we've since pushed a multiple of 16 bytes to the
     * stack since (pushed 0 bytes so far) and the alignment is thus
     * preserved and the call is well defined.
    */

    /* Restore variables */
    movl _perserve_magic, %eax
    movl _perserve_struct, %ebx

    /* Do some shit */
    addl $ARCH_VIRT_BASE, %ebx

    /* Push parameters */
    pushl %ebx
    pushl %eax

    call kernel_main
    call _fini

    /*
     * If the system has nothing more to do, put the computer into an
     * infinite loop. To do that:
     * 1) Disable interrupts with cli (clear interrupt enable in eflags).
     *      They are already disabled by the bootloader, so this is not needed.
     *      Mind that you might later enable interrupts and return from
     *      kernel_main (which is sort of nonsensical to do).
     * 2) Wait for the next interrupt to arrive with hlt (halt instruction).
     *      Since they are disabled, this will lock up the computer.
     * 3) Jump to the hlt instruction if it ever wakes up due to a
     *      non-maskable interrupt occurring or due to system management mode.
    */

    cli
0:	hlt
    jmp 0b

/*
 * Set the size of the _start symbol to the current location '.' minus its start.
 * This is useful when debugging or when you implement call tracing.
*/

.size _start, . - _start
