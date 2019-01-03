; Copyright (c) 2019 The cxkernel Authors. All rights reserved.
; Use of this source code is governed by a MIT-style
; license that can be found in the LICENSE file or at
; https://opensource.org/licenses/MIT
;
; @file   start.asm
; @author Kevin Dai \<kevindai02@outlook.com\>
; @date   Created on October 08 2018, 4:35 PM

[BITS 32]
ALIGN 4

; Declare constants for the multiboot header
ALIGNN   equ 1 << 0           ; align loaded modules on page boundaries
MEMINFO  equ 1 << 1           ; provide memory map
FLAGS    equ ALIGNN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ 0x1BADB002       ; 'magic number' lets bootloader find the header
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

[GLOBAL load_gdt]
load_gdt:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

[GLOBAL enable_xd]
enable_xd:
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 11
    wrmsr
