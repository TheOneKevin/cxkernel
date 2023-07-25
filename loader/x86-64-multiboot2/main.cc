#include "loader.h"
#include "assert.h"
#include <ebl/util.h>
#include <ebl/stdio.h>
#include <ebl/string.h>
#include <elf/elf.h>
#include <ebl/status.h>

//===----------------------------------------------------------------------===//
// Global variables and structures

// Define initial GDT entries
static ns::gdt_entry gdt[3] {
    ns::gdt_entry{}, // null
    ns::gdt_entry{0, 0, 0b1000}, // code
    ns::gdt_entry{0, 0, 0b0010}, // data
};

// Pointer to GDT
static ns::gdt_ptr gdt_ptr {
    .limit = sizeof(gdt) - 1,
    .base = (uint64_t) &gdt
};

//===----------------------------------------------------------------------===//
// Console functions

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

#define COM1 0x3F8
#define COM2 0x2F8

static void console_init(int port) {
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); // (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

namespace platform {

void console_emit(char c) {
    while((inb(COM1+5) & 0x20) == 0);
    outb(COM1, c);
}

void console_puts(char const* c) {
    while(*c) console_emit(*c++);
}

void console_log(char const* c) {
    while(*c) {
        while((inb(COM2+5) & 0x20) == 0);
        outb(COM2, *c++);
    }
}

} // namespace platform

//===----------------------------------------------------------------------===//
// Main entry point (called from start.s)

extern "C" void(*init_array_start_)();
extern "C" void(*init_array_end_)();
extern "C" void enter_longmode(
    uint32_t pml4_ptr, uint32_t gdt_ptr, uint64_t entry_point, uint32_t magic);

extern "C" [[noreturn]] void main(int sig, unsigned long ptr) {
    status_t ec = E::SUCCESS;

    // Run .init_array
    for(auto* fn = &init_array_start_; fn != &init_array_end_; fn++) (*fn)();

    // Initial checks
    (void) gdt_ptr;
    console_init(COM1);
    console_init(COM2);
    assert(sig == MULTIBOOT2_BOOTLOADER_MAGIC, "Invalid multiboot2 signature");

    // Search for kernel module
    struct multiboot_tag_module* module = nullptr;
    for(auto* tag : multiboot_tag_list {ptr, MULTIBOOT_TAG_TYPE_MODULE}) {
        auto* tmp = (decltype(module)) tag;
        if(ebl::strcmp(tmp->cmdline, "cxkernel") == 0) {
            module = tmp;
            break;
        }
    }

    // Check if kernel module was found
    assert(module != nullptr, "Kernel module (cxkernel) not found!");

    // Search for memory map
    struct multiboot_tag_mmap *mmap = nullptr;
    for (auto* tag : multiboot_tag_list {ptr, MULTIBOOT_TAG_TYPE_MMAP}) {
        mmap = (decltype(mmap)) tag;
        break;
    }
    assert(mmap != nullptr, "Memory map not found!");

    // Print the memory map
    ebl::klog("Memory map:\n");
    for(auto* entry : multiboot_mmap_list{mmap}) {
        ebl::klog("  %016lx - %016lx: %s\n", entry->addr,
                entry->addr + entry->len - 1,
                entry->type == MULTIBOOT_MEMORY_AVAILABLE ? "available" :
                entry->type == MULTIBOOT_MEMORY_RESERVED ? "reserved" :
                entry->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE ? "ACPI reclaimable" :
                entry->type == MULTIBOOT_MEMORY_NVS ? "NVS" :
                entry->type == MULTIBOOT_MEMORY_BADRAM ? "bad RAM" :
                "unknown");
    }

    // Initialize PMM and enable paging
    const range reserved[1] {
        range{module->mod_start, module->mod_end - 1}
    };
    bootstrap_pmm(reserved, mmap);
    auto pml4 = setup_paging();

    // Identity map the lower 2 MiB
    for(int i = 0; i < 0x200000; i += arch::page_size) {
        map_page(i, i, ns::page_flags{
            .f {
                .writable = 1,
                .user = 0
            }
        });
    }

    // Load and map the kernel ELF
    elf::Context ctx{};
    ec = elf::Context::load(ctx, (void*) module->mod_start);
    assert(ec, "Failed to load ELF");
    assert(ctx.is_64bits(), "Kernel must be 64-bit");
    for(auto& ph : ctx.get_header<elf64_phdr_t>()) {
        if(ph.p_type != PT_LOAD) continue;
        assert(ph.p_align == (uint64_t) arch::page_size,
            "Program section not aligned - all sections must be page-aligned!");
        vaddr_t v = ph.p_vaddr;
        paddr_t p = (paddr_t) ctx.get_image() + ph.p_offset;
        const ns::page_flags flags{
            .f {
                .writable = 1,
                .user = 0
            }
        };
        // Should we allocate more memory? Check filesz versus memsz.
        if(ph.p_filesz != ph.p_memsz) {
            // Get size of data within file
            size_t size = (size_t) ebl::max(ph.p_filesz, ph.p_memsz);
            for(uint64_t i = 0; i < arch::page_align_up(ph.p_memsz); i += arch::page_size) {
                // Allocate and map pages
                auto page = pmm_alloc_page();
                map_page(page, v+i, flags);
                // Copy data from file if possible
                if(i < size) {
                    ebl::memcpy((void*) page, (void*) ((uint32_t) p+i), arch::page_size);
                }
            }
        } else {
            for(uint64_t i = 0; i < ph.p_memsz; i += arch::page_size)
                map_page(p+i, v+i, flags);
        }
    }

    auto* img = static_cast<elf64_ehdr_t*>(ctx.get_image());
    ebl::kout("Jumping to kernel at %016lx\n", img->e_entry);
    enter_longmode(
        (uint32_t) pml4,
        (uint32_t) &gdt_ptr,
        (uint64_t) img->e_entry,
        (uint32_t) 0xDEADBEEF
    );

    for(;;);
}
