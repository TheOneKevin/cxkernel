/*
 * @file   linker.ld
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Tuesday, October 9th 2018, 5:04:24 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-28T13:57:14-04:00
 */

ENTRY(kernel_main)
SECTIONS
{
    . = 0xE0100000;
    _kernel_start = .;

    .text ALIGN (4K) : AT (ADDR (.text) - 0xE0000000)
    {
        *(.multiboot)
        *(.text*)
        *(.gnu.linkonce.t.*)
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xE0000000)
    {
        *(.rodata*)
        *(.gnu.linkonce.r.*)
    }

    .data ALIGN (4K) : AT (ADDR (.data) - 0xE0000000)
    {
        *(.data*)
        *(.gnu.linkonce.d.*)
    }

    .ctors : AT (ADDR (.ctors) - 0xE0000000)
    {
        _ctors_start = .;
        *(SORT(ctors*))
    }
    _ctors_end = .;

    .ksymtab        : AT (ADDR (.ksymtab)     - 0xE0000000) { *(SORT(ksymtab+*)) }
    .ksymtab_gpl    : AT (ADDR (.ksymtab_gpl) - 0xE0000000) { *(SORT(ksymtab_gpl+*)) }
    .kstrtab        : AT (ADDR (.kstrtab)     - 0xE0000000) { *(kstrtab) }

    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xE0000000)
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    _kernel_end = .;
}
