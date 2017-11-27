[EXTERN syscall_handler]
[GLOBAL syscall_asm_stub]
syscall_asm_stub:
    call syscall_handler
    iret
