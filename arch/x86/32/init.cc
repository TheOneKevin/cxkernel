/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 13 2018, 8:39 PM
 */

#define __MODULE__ "ARCH"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <panic.h>

#include "arch/x86/arch_utils.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/global.h"
#include "platform/interrupts.h"
#include "arch/interface.h"
#include "arch/x86/32/gdt.h"
#include "arch/x86/32/idt.h"

extern "C" uint32_t isr_stub_table;
extern "C" uint32_t _kernel_start;
extern "C" uint32_t _kernel_end;

namespace x86::g
{
    MEMORY_MAP mmap = {};
    elf::Context ctx = {};
    multiboot_info_t* mbt;
}

namespace x86_32
{
    // Reserve spaces for structs
    static multiboot_info_t mbt;
    void early_init(loader_t args)
    {
        // Save the multiboot info pointer in a global variable
        memcpy(&mbt, args.obj, sizeof(multiboot_info_t));
        x86::g::mbt = &mbt;
        memcpy(&x86::g::ctx, &args.ctx, sizeof(elf::Context));

        // Multiboot sanity check
        OS_PRN("%-66s", "Checking multiboot integrity... ");
        fflush(STREAM_OUT);
        ASSERT_HARD(args.magic == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
        ASSERT_HARD(!!CHECK_FLAG(mbt.flags, 0), "No memory information provided. Kernel cannot continue.");
        ASSERT_HARD(CHECK_FLAG(mbt.flags, 3) && mbt.mods_addr != 0 && mbt.mods_count > 0, "No module(s) loaded. Check GRUB config file.");
        ASSERT_HARD(!((CHECK_FLAG(mbt.flags, 4) && CHECK_FLAG(mbt.flags, 5))), "Flags 4 and 5 are mutually exclusive.");
        ASSERT_HARD(!!CHECK_FLAG(mbt.flags, 6), "Memory map not loaded. Kernel cannot continue execution.");
        fprintf(STREAM_OUT, "DONE!\n");

        // Collect basic information regarding kernel symbol sections, memory map and module locations
        OS_PRN("%-66s", "Collecting symbol data...");
            //PANIC("Kernel elf image is invalid!");
        fprintf(STREAM_OUT, "DONE!\n");

        ARCH_FOREACH_MMAP(mmap, x86::g::mbt, 0)
        {
            if(mmap -> addr == 0x100000 && !(mmap -> type == MULTIBOOT_MEMORY_AVAILABLE && mmap -> len > 0x4000000))
                PANIC("Not enough memory.\nNeeds at least 64 MB of continuous physical ram at 0x100000!\n");
            fprintf(STREAM_LOG, "  Entry address: 0x%016lX Entry length: 0x%016lX (0x%02X)\n", (uint64_t) mmap -> addr, (uint64_t) mmap -> len, (uint64_t) mmap -> type);
        }

        auto* mod = (multiboot_module_t *) mbt.mods_addr;
        OS_PRN("%d %s\n", mbt.mods_count, mbt.mods_count == 1 ? "module loaded." : "modules loaded.");
        for (uint32_t i = 0; i < mbt.mods_count; i++, mod++)
            OS_LOG("[%02d] Module location: From 0x%08X to 0x%08X\n", i + 1, mod -> mod_start, mod -> mod_end);
        mod--;

        // Initialize memory map
        x86::g::mmap.MOD_END = ARCH_VIRT_PHYS(mod -> mod_end);
        x86::g::mmap.KRN_BEGIN = (virt_t) &_kernel_start;
        x86::g::mmap.KRN_END = (virt_t) &_kernel_end;
        x86::g::mmap.KRN_BRK = x86::g::mmap.KRN_END;

        gdt::init();
        idt::init();
        for(int i = 0; i < 256; i++)
            idt::set_gate(i, (&isr_stub_table)[i], 0x08, 0x8E);
    }

    void init()
    {
        for(int i = 0; i < 32; i++)
            irq_install_handler(i, (irq_handler_t) &exception_handler);
        asm volatile("int $0x0");
    }
} // namespace x86_32
