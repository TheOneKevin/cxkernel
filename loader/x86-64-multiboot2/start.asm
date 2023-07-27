[BITS 32]
ALIGN 4

%define MULTIBOOT2_HEADER_MAGIC 0xe85250d6
%define GRUB_MULTIBOOT_ARCHITECTURE_I386 0
%define MULTIBOOT_HEADER_TAG_FRAMEBUFFER 5
%define MULTIBOOT_HEADER_TAG_OPTIONAL 1
%define MULTIBOOT_HEADER_TAG_END 0

; Declare constants for the multiboot header
; ref: https://forum.osdev.org/viewtopic.php?f=1&t=39370
; ref: https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
SECTION .multiboot ALIGN=4
multiboot_header_start:
    dd MULTIBOOT2_HEADER_MAGIC
    dd GRUB_MULTIBOOT_ARCHITECTURE_I386
    dd multiboot_header_end - multiboot_header_start
    dd -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + \
        (multiboot_header_end - multiboot_header_start))
ALIGN 8
framebuffer_tag_start:
    dw MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    dw MULTIBOOT_HEADER_TAG_OPTIONAL
    dd framebuffer_tag_end - framebuffer_tag_start
    dd 1024
    dd 768
    dd 32
framebuffer_tag_end:
ALIGN 8
    dw MULTIBOOT_HEADER_TAG_END
    dw 0
    dd 8
multiboot_header_end:

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
SECTION .bootloader_stack nobits alloc write ALIGN=16
[GLOBAL stack_top_]
[GLOBAL stack_bottom_]
stack_bottom_:
    resb 1024
stack_top_:

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
SECTION .text
[GLOBAL _start]
[EXTERN main]

_start:
    cli
    mov esp, stack_top_
    push 0
    popf                    ; Reset EFLAGS
    push ebx                ; Set up stack & push grub multiboot structs
    push eax
    call main               ; Call our C code
    cli
.0b:
    hlt
    jmp .0b

[GLOBAL enter_longmode]
enter_longmode:
    xchg bx, bx
    mov eax, [esp+4]
    mov cr3, eax
    mov eax, cr4
    or  eax, 0x20           ; CR4.PAE
    mov cr4, eax
    mov ecx, 0xC0000080     ; IA32_EFER MSR, ref 2.2.1 SDM Vol 3
    rdmsr
    or  eax, 0x100          ; IA32_EFER.LME
    wrmsr
    mov eax, cr0
    or  eax, 0x80000001     ; CR0.PE,PG ref Figure 2-7 SDM Vol 3
    mov cr0, eax
    mov eax, [esp+8]        ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]              ; Load the new GDT pointer
    jmp 0x08:_start64       ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

[BITS 64]
[GLOBAL _start64]
_start64:
    xchg bx, bx
    mov ax, 0x10            ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax              ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rdi, QWORD [esp+20] ; Load kernel arg0
    mov rax, QWORD [esp+12]
    mov rsp, QWORD [esp+28] ; Load stack virtual address
    call rax                ; Jump to the kernel's entry point!
    cli
    hlt
.0b:
    jmp .0b
