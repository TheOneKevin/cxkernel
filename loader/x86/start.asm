; @file   start.asm
; @author Kevin Dai \<kevindai02@outlook.com\>
; 
; @date Created on Monday, October 8th 2018, 4:35:25 pm
; 
; @date Last modified by:   Kevin Dai
; @date Last modified time: 2018-10-28T15:25:05-04:00

[BITS 32]
ALIGN 4

; Declare constants for the multiboot header
ALIGNN   equ 1 << 0           ; align loaded modules on page boundaries
MEMINFO  equ 1 << 1           ; provide memory map
FLAGS    equ ALIGNN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002      ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

ARCH_PAGE_SIZE equ 0x1000

; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
SECTION .multiboot ALIGN=4
dd MAGIC
dd FLAGS
dd CHECKSUM

; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
SECTION .bootloader_stack nobits alloc write ALIGN=4
[GLOBAL stack_top]
[GLOBAL stack_bottom]
stack_bottom:
    resb 4096 ; This should be enough
stack_top:

; Paging tables and directories for higher half kernel
; Statically allocated for temporary usage
SECTION .bss nobits alloc write

[GLOBAL _kernel_dir1]
[GLOBAL _kernel_dir2]
[GLOBAL _kernel_dir3]
[GLOBAL _kernel_dir4]
[GLOBAL _kernel_table1]
[GLOBAL _kernel_table2]
[GLOBAL _kernel_table3]
[GLOBAL _kernel_table4]
[GLOBAL _kernel_table5]
[GLOBAL _kernel_table6]
[GLOBAL _kernel_table7]

align 0x1000
_kernel_dir1: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_dir2: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_dir3: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_dir4: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table1: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table2: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table3: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table4: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table5: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table6: resb ARCH_PAGE_SIZE
align 0x1000
_kernel_table7: resb ARCH_PAGE_SIZE

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
SECTION .text
[GLOBAL _start]
[EXTERN main]

_start:
    ; Oh how I love NASM
    cli
    mov esp, stack_top ; Set up stack
    push ebx ; Push grub multiboot structs
    push eax
    call main
    cli
.0b:
    hlt
    jmp .0b
