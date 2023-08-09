[BITS 64]

SECTION .text

[GLOBAL load_idt]
load_idt:
    lidt [rdi]
    ret

%assign i 0
%rep 256
    %if (i = 8) || (i >= 10 && i <= 14) || (i = 17) || (i = 20) || (i = 30)
        ;[GLOBAL isr%+i]
        isr%+i:
            cli
            push i
            jmp irq_stub
    %else
        ;[GLOBAL isr%+i]
        isr%+i:
            cli
            push 0
            push i
            jmp irq_stub
    %endif
    %assign i i+1
%endrep

[EXTERN isr_handler]

irq_stub:
    cld
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi

    mov rdi, rsp    ; Pass the stack pointer as the first argument

    call isr_handler

    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    add rsp, 16     ; Cleans up the pushed error code and interrupt number
    iret

SECTION .data

[GLOBAL isr_stub_table]
isr_stub_table:
%assign i 0
%rep 256
    dq isr%+i
    %assign i i+1
%endrep
