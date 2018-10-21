# 1 "abi/arch/x86/crtn.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "abi/arch/x86/crtn.S"
# 11 "abi/arch/x86/crtn.S"
.section .init
    popl %ebp
    ret

.section .fini
    popl %ebp
    ret
