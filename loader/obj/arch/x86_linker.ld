/*
 * @file   linker.ld
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Tuesday, October 9th 2018, 5:04:24 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-11T09:39:23-05:00
 */

ENTRY(_start)
SECTIONS
{
    . = 0x100000;
    _lodr_start = .;

    .text ALIGN (4K) : AT (ADDR (.text))
    {
        *(.multiboot)
        *(.text*)
        *(.gnu.linkonce.t.*)
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata))
    {
        *(.rodata*)
        *(.gnu.linkonce.r.*)
    }

    .ctors : AT (ADDR (.ctors))
    {
        _ctors_start = .;
        *(SORT(ctors*))
    }
    _ctors_end = .;

    .data ALIGN (4K) : AT (ADDR (.data))
    {
        *(.data*)
        *(.gnu.linkonce.d.*)
    }

    .bss ALIGN (4K) : AT (ADDR (.bss))
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    _lodr_end = .;
}
