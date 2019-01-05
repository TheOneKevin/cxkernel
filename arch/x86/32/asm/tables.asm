; Copyright (c) 2019 The cxkernel authors. All rights reserved.
; Use of this source code is governed by a MIT-style
; license that can be found in the LICENSE file or at
; https://opensource.org/licenses/MIT
;
; @file   tables.asm
; @author Kevin Dai \<kevindai02@outlook.com\>
; @date   Created on October 27 2018, 7:53 PM

[BITS 32]

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
