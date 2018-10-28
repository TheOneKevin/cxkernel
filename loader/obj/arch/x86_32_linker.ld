/*
 * @file   linker.ld
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Tuesday, October 9th 2018, 5:04:24 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-28T13:55:56-04:00
 */

ENTRY(_start)
SECTIONS
{
    . = 0x100000;
    _kernel_start = .;

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

    _kernel_end = .;
}
