/*
 * File:   init.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 12:17:34 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:25:51-04:00
*/

#define __MODULE__ "INIT"

#include "arch/x86/global.h"
#include "arch/x86/multiboot.h"

#include "arch/arch_interface.h"
#include "arch/debug.h"

#include "arch/x86/arch_common.h"
#include "arch/x86/multiboot_utils.h"
#include "arch/x86/gdt.h"
#include "arch/x86/syscalls.h"
#include "arch/x86/exceptions.h"
#include "arch/x86/interrupts.h"
#include "arch/x86/vga.h"
#include "arch/x86/serial.h"
#include "arch/x86/acpi.h"
#include "arch/x86/apic.h"
#include "arch/x86/bootmm.h"

#include "lib/lib.h"
#include "panic.h"
#include "assert.h"
#include "mm/malloc.h"
#include "lib/printk.h"
#include "lib/string.h"
#include "lib/tui/BasicDrawUtils.h"

#include "core/console.h"
#include "tasking/symbol_table.h"

extern void __initKernelTests__();
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

// Reserve spaces for structs
static multiboot_info_t mbt;
static symbol_table_desc_t _sym_table;

// Consoles
extern console_t __internal_vga_cons;
extern console_t __internal_serial_cons;

console_t* arch_get_console(void)
{
    init_early_handles();
    init_serial_console();
    __internal_vga_cons.getc = __internal_serial_cons.getc;
    return &__internal_vga_cons;
}

/**
 * Early archietecture initialization procedure. Sets up system
 * and collects information from bootloader. Mainly here to bootstrap
 * the physical and virtual memory managers.
 * @param magic Magic boot verification number.
 * @param ptr   Pointer to any boot data structure(s).
 */
void arch_early_init(uint32_t magic, void* ptr)
{
    // Save the multiboot info pointer in a global variable
    memcpy(&mbt, ptr, sizeof(multiboot_info_t));
    g_mbt_struct = &mbt;

    OS_PRN("%-66s", "Checking multiboot integrity... ");
    fflush(STREAM_OUT);
    // Multiboot sanity check
    ASSERT_HARD(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 0), "No memory information provided. Kernel cannot continue.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 3) && mbt.mods_addr != 0 && mbt.mods_count > 0, "No module(s) loaded. Check GRUB config file.");
    ASSERT_HARD(!((CHECK_FLAG(mbt.flags, 4) && CHECK_FLAG(mbt.flags, 5))), "Flags 4 and 5 are mutually exclusive.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 6), "Memory map not loaded. Kernel cannot continue execution.");
    fprintf(STREAM_OUT, "DONE!\n");
    
    // Collect basic information regarding kernel symbol sections, memory map and module locations
    OS_PRN("%-66s", "Collecting symbol data...");
    if(mb_elf_get_section_header(".symtab") != 0 && mb_elf_get_section_header(".strtab") != 0)
    {
        g_krnl_sym_table = &_sym_table;
        g_krnl_sym_table -> present = true;
        g_krnl_sym_table -> num_sym = mb_elf_get_section_header(".symtab") -> sh_size / sizeof(elf_sym_t);
        g_krnl_sym_table -> symbols = (elf_sym_t *) ARCH_VIRT_PHYS(mb_elf_get_section_header(".symtab") -> sh_addr);
        g_krnl_sym_table -> string_table_addr = (char *) ARCH_VIRT_PHYS(mb_elf_get_section_header(".strtab") -> sh_addr);
    }
    else
        PANIC("Kernel elf image is invalid!");
    fprintf(STREAM_OUT, "DONE!\n");

    ARCH_FOREACH_MMAP(mmap)
    {
        if(mmap -> addr == 0x100000 && !(mmap -> type == MULTIBOOT_MEMORY_AVAILABLE && mmap -> len > 0x4000000))
            PANIC("Not enough memory.\nNeeds at least 64 MB of continuous physical ram at 0x100000!\n");
        fprintf(STREAM_LOG, "  Entry address: 0x%016lX Entry length: 0x%016lX (0x%02X)\n", (uint64_t) mmap -> addr, (uint64_t) mmap -> len, (uint64_t) mmap -> type);
    }

    multiboot_module_t* mod = (multiboot_module_t *) ARCH_VIRT_PHYS(mbt.mods_addr);
    OS_PRN("%d %s\n", mbt.mods_count, mbt.mods_count == 1 ? "module loaded." : "modules loaded.");
    for (uint32_t i = 0; i < mbt.mods_count; i++, mod++)
        OS_LOG("[%02d] Module location: From 0x%08X to 0x%08X\n", i + 1, ARCH_VIRT_PHYS(mod -> mod_start), ARCH_VIRT_PHYS(mod -> mod_end));
    mod--;

    // Initialize memory map
    g_memory_map.MOD_END = ARCH_VIRT_PHYS(mod -> mod_end);
    g_memory_map.KRN_BEGIN = (virt_t) &_kernel_start;
    g_memory_map.KRN_END = (virt_t) &_kernel_end;
    g_memory_map.KRN_BRK = g_memory_map.KRN_END;

    // Install interrupt handlers
    install_gdt();
    install_idt();

#ifdef DEBUG
    initialize_debugger();
#else
    install_isr();
#endif

    // Initialize ACPI boot time tables, and then deinitialize it
    initTmpBootACPI();
    getCPUInfo();
    enable_APIC();
    deinitTmpBootACPI();
}

#include "list.h"

typedef struct
{
    list_head_t list;
    int id;
} myStruct;

void arch_init(void)
{
    myStruct* List = (myStruct *) malloc(sizeof(myStruct));
    List -> id = 0;
    INIT_LIST_HEAD(&(List -> list));
    list_head_t* ptr = &(List -> list);
    for(int i = 0; i < 5; i++)
    {
        myStruct* entry = (myStruct *) malloc(sizeof(myStruct));
        list_add(&(entry -> list), ptr);
        INIT_LIST_HEAD(&(entry -> list));
        entry -> id = i + 1;
        ptr = &(entry -> list);
    }

    myStruct* ent = NULL;
    iforeach_list_entry(ent IN list OF List)
    {
        kprintf("%d\n", ent -> id);
    }
    malloc_stats();
}

void arch_late_init(void)
{
    syscalls_register();

    #ifdef KERNEL_SELF_TEST
        fprintf(STREAM_OUT, "Initiating kernel integrity self tests...");
        fflush(STREAM_OUT);
        __initKernelTests__();
        fprintf(STREAM_OUT, " Done!\n");
    #endif
}
