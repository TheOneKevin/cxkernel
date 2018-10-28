# 1 "loader/arch/x86/start.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "loader/arch/x86/start.S"

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)


# 1 "include/arch/x86/arch_common.h" 1
# 11 "include/arch/x86/arch_common.h"
       
# 10 "loader/arch/x86/start.S" 2
# 1 "include/arch/x86/interface/arch_interface.h" 1
# 11 "include/arch/x86/interface/arch_interface.h"
       
# 11 "loader/arch/x86/start.S" 2
# 21 "loader/arch/x86/start.S"
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
# 40 "loader/arch/x86/start.S"
.section .bootloader_stack, "aw", @nobits
.align 0x1000
.global stack_top
.global stack_bottom
stack_bottom:
.skip 0x1000 # 4 KB
stack_top:



.section .bss, "aw", @nobits
_perserve_magic:
    .skip 4
_perserve_struct:
    .skip 4

.align 0x1000
_kernel_dir1:
    .skip 0x1000
_kernel_table1:
    .skip 0x1000
_kernel_table2:
    .skip 0x1000
_kernel_table3:
    .skip 0x1000
_kernel_table4:
    .skip 0x1000
_kernel_table5:
    .skip 0x1000
_kernel_table6:
    .skip 0x1000
_kernel_table7:
    .skip 0x1000

_kernel_dir2:
    .skip 0x1000
_kernel_dir3:
    .skip 0x1000
_kernel_dir4:
    .skip 0x1000

.global _kernel_dir1
.global _kernel_dir2
.global _kernel_dir3
.global _kernel_dir4
.global _kernel_table1
.global _kernel_table2
.global _kernel_table3
.global _kernel_table4
.global _kernel_table5
.global _kernel_table6
.global _kernel_table7







.section .text
.global _start
.type _start, @function

_start:

    cli


    movl %eax, _perserve_magic - 0xE0000000
    movl %ebx, _perserve_struct - 0xE0000000

    movl $(_kernel_table1 - 0xE0000000), %edi
    movl $0, %esi
    movl $1024, %ecx
1:
    movl %esi, %edx
    orl $0x3, %edx
    movl %edx, (%edi)
    addl $0x1000, %esi
    addl $4, %edi
    loop 1b
3:
    movl $(_kernel_table2 - 0xE0000000), %edi
    movl $(stack_bottom - 0xE0000000), %edx
    orl $0x3, %edx
    movl $(_kernel_table2 - 0xE0000000 + 4 * 1023), %edi
    movl %edx, (%edi)

    movl $(_kernel_table1 - 0xE0000000 + 0x3), _kernel_dir1 - 0xE0000000 + 0

    movl $(_kernel_table1 - 0xE0000000 + 0x3), _kernel_dir1 - 0xE0000000 + (0xE0000000 >> 22) * 4

    movl $(_kernel_table2 - 0xE0000000 + 0x3), _kernel_dir1 - 0xE0000000 + 0xFF4

    movl $(_kernel_dir1 - 0xE0000000), %ecx
    movl %ecx, %cr3
    movl %cr0, %ecx
    orl $0x80010001, %ecx
    movl %ecx, %cr0

    lea 4f, %ecx
    jmp *%ecx
4:
# 159 "loader/arch/x86/start.S"
    mov $0xFF800000, %esp
# 180 "loader/arch/x86/start.S"
    movl _perserve_magic, %eax
    movl _perserve_struct, %ebx


    addl $0xE0000000, %ebx


    pushl %ebx
    pushl %eax

    call kernel_main
# 205 "loader/arch/x86/start.S"
    cli
0: hlt
    jmp 0b






.size _start, . - _start
