# 1 "abi/arch/x86/crti.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "abi/arch/x86/crti.S"
# 11 "abi/arch/x86/crti.S"
.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp

.section .fini
.global _fini
.type _fini @function
_fini:
    push %ebp
    movl %esp, %ebp
