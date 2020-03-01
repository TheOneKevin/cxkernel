/**
 * Copyright (c) 2020 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on February 29 2020, 11:12 AM
 */

/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 05 2019, 5:19 PM
 */

#include <string.h>
#include <panic.h>
#include <stdio.h>
#include <math.h>

#include "core/memory.h"
#include "arch/x86/global.h"
#include "arch/x86/arch_utils.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/cpu.h"
#include "platform.h"
#include "platform/interrupts.h"
#include "platform/console.h"
#include "platform/pc/vga.h"
#include "platform/pc/pic.h"
#include "platform/pc/serial.h"

#if ARCH_TYPE == ARCH_x86_32
#include "arch/x86/32/idt.h"
#elif ARCH_TYPE == ARCH_x86_64
#include "arch/x86/64/idt.h"
#endif

namespace pc
{
    namespace console
    {
        class ConsoleImpl : public platform::Console
        {
        public:
            void log(char c) override
            {
                serial::write(COM1, c);
            }
            void putc(char c) override
            {
                vga::putc(c);
            }
            void clear(void) override
            {
                vga::clear(0);
            }
            char getc(void) override
            {
                return Console::getc();
            }
        };
        static ConsoleImpl __internal_vga_cons;
    }  // namespace console

    namespace interrupts
    {
        static irq_handler_t handlers[255]; // Should be enough
        class IrqImpl : public platform::Irq
        {
        public:
            void install_handler(int n, irq_handler_t handler) override
            {
                handlers[n] = handler;
            }

            void remove_handler(int n) override
            {
                handlers[n] = NULL;
            }

            irq_handler_t get_handler(int n) override
            {
                return handlers[n];
            }
        };
        static IrqImpl __internal_irq;
    }  // namespace interrupts
}  // namespace pc

namespace platform
{
    using namespace pc;

    static pmm_arena_t arena =
    {
#if ARCH_TYPE == ARCH_x86_32
        .node = { NULL, NULL },
        .flags = CX_ARENA_PRESENT,
        .base = 0,
        .size = 0,
        .free = 0,
        .priority = 1,
        .pages = NULL,
        .free_list = { NULL, NULL }
#endif
    };

    static pmm_arena_t kmap =
    {
#if ARCH_TYPE == ARCH_x86_32
        .node = { NULL, NULL },
        .flags = CX_ARENA_PRESENT | CX_ARENA_KERNEL,
        .base = 0,
        .size = 0,
        .free = 0,
        .priority = 2,
        .pages = NULL,
        .free_list = { NULL, NULL }
#endif
    };

    Console &console = static_cast<Console &>(console::__internal_vga_cons);
    Irq& irq = static_cast<Irq &>(interrupts::__internal_irq);

    void early_init(void)
    {
        // PIC and other PC stuff
        pic::remap(IRQ0, IRQ0 + 8);
        pic::mask(0xFF, 0xFF);
        serial::init(COM1);
        serial::init(COM2);
        memset(interrupts::handlers, 0, sizeof(interrupts::handlers));
    }

    static phys_t max_pa = 0;
    void init(void)
    {
        // Memory Topology
        ARCH_FOREACH_MMAP(mmap, x86::g::mbt, 0)
            x86::g::max_mem = MAX(x86::g::max_mem, mmap->addr + mmap->len);
        
        // Map the lower XX MiB/GiB into upper memory
        virt_t va = 0;
        ARCH_FOREACH_MMAP(mmap, x86::g::mbt, 0)
        {
            if(va >= ARCH_KMALLOC_LENGTH) break;

            if(!x86_feature_test(x86_FEATURE_PAE) && mmap->addr >= 0xFFFFFFFFUL)
                break;
            if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
            {
                phys_t start_addr = ARCH_PAGE_ALIGN(mmap->addr);
                phys_t end_addr = ARCH_PAGE_ALIGN_DOWN(mmap->addr + mmap->len);
                
                // TODO: We waste 1 MiB of ram by mapping the lower 16 MiB to 0xC000`0000
                // and the lower 1 MiB to 0xE000`0000, see kmem_init()
                for(phys_t pa = start_addr; (pa < end_addr) && (va < ARCH_KMALLOC_LENGTH);)
                {
                    arch::get_mmu().map((uint64_t) va + ARCH_KMALLOC_BASE, pa, 0x3);
                    pa += ARCH_PAGE_SIZE, va += ARCH_PAGE_SIZE;
                    max_pa = MAX(pa, max_pa);
                }
            }
        }

        BOCHS_MAGIC_BREAK();
    }

    void meminit(void)
    {
        // Initialize data structures
        kmap.base = 0;
        kmap.size = max_pa / ARCH_PAGE_SIZE;
        kmap.pages = (page_t*) (g::loader -> pps_start);
        pmm::get_allocator().AddArena(&kmap, g::loader -> bitmap);

        /*arena.base = max_pa;
        arena.size = (uint32_t)((ARCH_PAGE_ALIGN(x86::g::max_mem) - max_pa) / ARCH_PAGE_SIZE);
        arena.pages = &((page_t*) g::loader -> pps_start)[max_pa / ARCH_PAGE_SIZE];
        pmm::get_allocator().AddArena(&arena, g::loader -> bitmap);*/
    }
} // namespace platform