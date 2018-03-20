[GLOBAL enable_pae_asm]

; cdecl: void enable_pae_asm(uint32_t);
enable_pae_asm:
    xchg bx, bx
    push ebp
    ; Set up new stack frame
    mov ebp, esp
    push eax

    ; Store parameter in ebx
    mov ebx, dword[ebp + 8] ; Get parameter

    ; Jump to the no_paging label located in the identity-mapped area
    mov ecx, no_paging
    sub ecx, 0xE0000000 ; ARCH_VIRT_BASE
    jmp ecx

no_paging:
    ; Disable paging
    mov eax, cr0
    and eax, 0x7FFFFFFF
    mov cr0, eax
    ; Set CR3 to the PDPT stored in ebx
    mov cr3, ebx
    ; Enable bit 5 in CR4 for PAE
    mov eax, cr4
    or eax, 0x20
    mov cr4, eax
    ; Enable paging again
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ; Return
    jmp done

done:
    ; Restore registers
    pop eax
    pop ebp
    ; Return
    ret
