ENTRY(kernel_entry)
SECTIONS
{
    . = 0xffffffff80000000;
    _kernel_start = .;

    .text ALIGN (4K) :
    {
        *(.multiboot)
        *(.text*)
        *(.gnu.linkonce.t.*)
    }

    .rodata ALIGN (4K) :
    {
        *(.rodata*)
        *(.gnu.linkonce.r.*)
    }

    .data ALIGN (4K) :
    {
        *(.data*)
        *(.gnu.linkonce.d.*)
    }

    .kstrtab : { *(kstrtab) }

    .bss ALIGN (4K) :
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    _kernel_end = .;
}
