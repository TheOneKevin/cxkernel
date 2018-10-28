; @file   tables.asm
; @author Kevin Dai \<kevindai02@outlook.com\>
; 
; @date Created on Saturday, October 27th 2018, 7:53:15 pm
; 
; @date Last modified by:   Kevin Dai
; @date Last modified time: 2018-10-27T19:53:31-04:00

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

[GLOBAL load_idt]     ; Allows the C code to call idt_flush().
load_idt:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt [eax]        ; Load the IDT pointer.
    ret

[GLOBAL load_tss]
load_tss:
    mov ax, 0x2B
    ltr ax
    ret
