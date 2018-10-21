/*
 * @file   linker.ld
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Tuesday, October 9th 2018, 5:04:24 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-19T15:59:15-04:00
 */

ENTRY(_start)
SECTIONS
{
    . = 0xE0100000;
    _kernel_start = .;

    .text ALIGN (4K) : AT (ADDR (.text) - 0xE0000000)
    {
        *(.multiboot)
        *(.text)
        *(.gnu.linkonce.t.*)

        _ctors_start = .;
        *(SORT(.ctor*))
        _ctors_end = .;

        _dtors_start = .;
        *(SORT(.dtor*))
        _dtors_end = .;
    }
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xE0000000)
    {
        *(.rodata)
        *(.gnu.linkonce.r.*)
    }

    .data ALIGN (4K) : AT (ADDR (.data) - 0xE0000000)
    {
        *(.data)
        *(.gnu.linkonce.d.*)
    }

    .ksymtab        : AT (ADDR (.ksymtab)     - 0xE0000000) { *(SORT(ksymtab+*)) }
    .ksymtab_gpl    : AT (ADDR (.ksymtab_gpl) - 0xE0000000) { *(SORT(ksymtab_gpl+*)) }
    .kstrtab        : AT (ADDR (.kstrtab)     - 0xE0000000) { *(kstrtab) }

    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xE0000000)
    {
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    _kernel_end = .;
}
