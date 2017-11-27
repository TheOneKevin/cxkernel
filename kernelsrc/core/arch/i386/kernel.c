/*
 * Filename: kernel.c
 *
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 12:17:34 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 08:19:33 PM
*/

#include "multiboot.h"

#include "arch/i386/common.h"
#include "arch/i386/sys/vga.h"
#include "arch/i386/sys/gdt.h"
#include "arch/i386/sys/syscalls.h"
#include "arch/i386/sys/exceptions.h"
#include "arch/i386/sys/interrupts.h"
#include "arch/i386/sys/multiboot_utils.h"

#include "lib/lib.h"
#include "lib/panic.h"
#include "lib/string.h"
#include "lib/printk.h"
#include "lib/assert.h"
#include "lib/tui/BasicDrawUtils.h"

#include "sys/malloc.h"
#include "sys/symbol_table.h"

extern void BeforeInit();
extern void Init();
extern void PostInit();

#if KERNEL_TEST == 1
    extern void __initKernelTests__();
#endif

extern void __initKernelTests__();
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

console_t cons0;
tty_t tty0 = { .console = &cons0 };
symbol_table_desc_t _sym_table;

void kernel_beforemain()
{
    BeforeInit();
}

void kernel_main(uint32_t magic, multiboot_info_t* mbt)
{
    __GGMBT__ = mbt; // Set the multiboot information structure address

    _ttys[0] = &tty0;
    _current_tty_id = 0;
    terminal_initialize(&cons0);

    // Multiboot sanity check
    ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
    ASSERT(CHECK_FLAG(mbt -> flags, 0), "No memory information provided. Kernel cannot continue.");
    ASSERT(CHECK_FLAG(mbt -> flags, 3) && mbt -> mods_addr != 0 && mbt -> mods_count > 0, "No module(s) loaded. Check GRUB config file.");
    ASSERT(!((CHECK_FLAG(mbt -> flags, 4) && CHECK_FLAG(mbt -> flags, 5))), "Flags 4 and 5 are mutually exclusive.");
    ASSERT(CHECK_FLAG(mbt -> flags, 6), "Memory map not loaded. Kernel cannot continue execution.");

    multiboot_module_t* mod = (multiboot_module_t *) (mbt -> mods_addr + VIRT_BASE);
    kprintf("%d module(s) loaded.\n", mbt -> mods_count, mod -> mod_start + VIRT_BASE);
    for (uint32_t i = 0; i < mbt -> mods_count; i++, mod++)
        kprintf("[%02d] Module location: 0x%08X - 0x%08X\n", i + 1, mod -> mod_start + VIRT_BASE, mod -> mod_end + VIRT_BASE);

    multiboot_memory_map_t* mmap = (multiboot_memory_map_t *) (mbt -> mmap_addr + VIRT_BASE);
    while ((uint32_t) mmap < mbt -> mmap_addr + mbt -> mmap_length + VIRT_BASE)
    {
        kprintf("  Entry address: 0x%016lX Entry length: 0x%016lX (0x%02X)\n", (uint64_t) mmap -> addr, (uint64_t) mmap -> len, (uint64_t) mmap -> type);
        mmap = (multiboot_memory_map_t *) ((unsigned int) mmap + mmap -> size + sizeof(mmap -> size));
    }

    // Set up kernel heap
    void* m = (void *) 0xE0200000; //(void *) PAGE_ALIGN(mod -> mod_end);
    mrvn_memory_init(m, 0x1000);

    if(load_symbol_table(mb_elf_get_section_header(".symtab"), mb_elf_get_section_header(".strtab"), &_sym_table) == 0)
        krnl_sym_table = &_sym_table;
    else
        for(;;);

    kprintf("%s\n", cpu_detect());

    install_gdt();
    install_idt();
    install_isr();
    syscalls_register();

    #if KERNEL_SELF_TEST == 1
        kprintf("Initiating kernel integrity self tests...");
        __initKernelTests__();
        kprintf(" Done!\n");
    #endif

    /*void* m = (void *) 0xE0200000;
       mrvn_memory_init(m, 0x100); */

    Init();
    PostInit();
}
