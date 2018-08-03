[BITS 32]
ALIGN 4

; Declare constants for the multiboot header
ALIGNN   equ 1 << 0           ; align loaded modules on page boundaries
MEMINFO  equ 1 << 1           ; provide memory map
FLAGS    equ ALIGNN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002      ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

; Map our kernel to the higher half
VIRTUAL_BASE equ 0xE0000000
KRNL_PAGE_NUMBER equ (VIRTUAL_BASE >> 22)
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
    resb ARCH_PAGE_SIZE ; 4KB
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

perserve_magic: resd 1
perserve_struct: resd 1

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
SECTION .text
[GLOBAL _start]
[EXTERN kernel_main]

_start:
    ; Oh how I love NASM
    cli
    ; Save the GRUB registers
    mov [perserve_magic - VIRTUAL_BASE], eax
    mov [perserve_struct - VIRTUAL_BASE], ebx

    mov edi, _kernel_table1 - VIRTUAL_BASE
    mov esi, 0
    mov ecx, 1024
.1b:
    mov edx, esi    ; ESI is the physical address
    or edx, 0x3     ; Mark as present and writable
    mov [edi], edx  ; Write to page table
    add esi, ARCH_PAGE_SIZE ; Increment our pointer to the current mapped address
    add edi, 4              ; Increment our pointer to the current entry in the table
    loop .1b
.2:
    mov edi, _kernel_table2 - VIRTUAL_BASE
    mov edx, stack_bottom - VIRTUAL_BASE
    or edx, 0x3
    mov edi, _kernel_table2 - VIRTUAL_BASE + 4 * 1023
    mov [edi], edx
    mov DWORD[_kernel_dir1 - VIRTUAL_BASE],                        _kernel_table1 - VIRTUAL_BASE + 0x3 ; Identity map the kernel table 1
    mov DWORD[_kernel_dir1 - VIRTUAL_BASE + KRNL_PAGE_NUMBER * 4], _kernel_table1 - VIRTUAL_BASE + 0x3 ; Map kernel to virt base
    mov DWORD[_kernel_dir1 - VIRTUAL_BASE + 0xFF4],                _kernel_table2 - VIRTUAL_BASE + 0x3 ; Map stack to 0xFF800000

    ; Mov address of the PD to CR3
    mov ecx, _kernel_dir1 - VIRTUAL_BASE
    mov cr3, ecx
    mov ecx, cr0
    or ecx, 0x80010001
    mov cr0, ecx ; Enable paging!
    
    mov eax, .3f
    jmp eax ; Go go go!
.3f:
    mov esp, 0xFF800000 ; Set up stack
    mov eax, [perserve_magic] ; Restore structs
    mov ebx, [perserve_struct]
    add ebx, VIRTUAL_BASE
    push ebx
    push eax
    call kernel_main
    cli
.0b:
    hlt
    jmp .0b
