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
