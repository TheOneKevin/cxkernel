/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
    /* Begin putting sections at 3.5 GB + 1 MiB, which will be mapped to the 1 MiB physical space */
    . = 0xE0100000;
    _kernel_start = .;

    /* First put the multiboot header, as it is required to be put very early
       early in the image or the bootloader won't recognize the file format.
       Next we'll put the .text section. */
    .text ALIGN (4K) : AT (ADDR (.text) - 0xE0000000)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xE0000000)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data ALIGN (4K) : AT (ADDR (.data) - 0xE0000000)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xE0000000)
    {
        *(COMMON)
        *(.bss)
        *(.bootloader_stack)
    }

    .ksymtab        : AT (ADDR (.ksymtab)       - 0xE0000000) { *(SORT(ksymtab+*)) }
    .ksymtab_gpl    : AT (ADDR (.ksymtab_gpl)   - 0xE0000000) { *(SORT(ksymtab_gpl+*)) }
    .kstrtab        : AT (ADDR (.kstrtab)       - 0xE0000000) { *(kstrtab) }

    /* This is where we get the kernel's end address */
    _kernel_end = .;
}
