ENTRY(_start)
SECTIONS
{
    . = 0x100000;
    loader_begin_ = .;

    .text ALIGN (4K):
    {
        *(.multiboot)
        *(.text*)
        *(.gnu.linkonce.t.*)
    }

    .rodata ALIGN (4K):
    {
        *(.rodata*)
        *(.gnu.linkonce.r.*)
    }

    .init_array :
    {
        init_array_start_ = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array ))
        init_array_end_ = .;
    }

    .data ALIGN (4K):
    {
        *(.data*)
        *(.gnu.linkonce.d.*)
    }

    .bss ALIGN (4K):
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(.bootloader_stack)
    }

    loader_end_ = .;
}
