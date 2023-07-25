ENTRY(kernel_entry)
SECTIONS
{
    . = 0xffff800000000000;
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

    .kstrtab ALIGN (4K) :
    {
        *(kstrtab)
    }

    .init_array ALIGN (4K) :
    {
        init_array_start_ = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array ))
        init_array_end_ = .;
    }

    .bss ALIGN (4K) :
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    _kernel_end = .;
}
