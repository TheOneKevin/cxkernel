; Copyright (c) 2019 The cxkernel authors. All rights reserved.
; Use of this source code is governed by a MIT-style
; license that can be found in the LICENSE file or at
; https://opensource.org/licenses/MIT
;
; @file   isr.asm
; @author Kevin Dai \<kevindai02@outlook.com\>
; @date   Created on January 04 2019, 9:49 AM

[BITS 32]
SECTION .text
%assign i 0
%rep 256
    %if (i = 8) || (i >= 10 && i <= 14) || (i = 17) || (i = 20) || (i = 30)
        ;[GLOBAL isr%+i]
        isr%+i:
            cli
            push i
            jmp int_stub
    %else
        ;[GLOBAL isr%+i]
        isr%+i:
            cli
            push 0
            push i
            jmp int_stub
    %endif
    %assign i i+1
%endrep

[EXTERN isr_handler]

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
int_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    cld
    push esp       ; Push us the stack
    call isr_handler
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

SECTION .data

[GLOBAL isr_stub_table]
isr_stub_table:
%assign i 0
%rep 256
    dd isr%+i
    %assign i i+1
%endrep
