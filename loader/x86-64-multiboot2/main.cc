#include "loader.h"
#include "assert.h"
#include <ebl/util.h>
#include <ebl/stdio.h>
#include <ebl/string.h>

//===----------------------------------------------------------------------===//
// Global variables and structures
// NOTE: System V ABI gaurentees bitfield order in packed structs,
//       even though the C++ standard does not.

// Figure 3.8 from Intel SDM 3.4.5
struct PACKED gdt_entry {
    uint16_t limit_low      : 16;
    uint16_t base_low       : 16;
    uint8_t  base_middle    : 8;
    uint8_t  seg_type       : 4;
    uint8_t  desc_type      : 1;
    uint8_t  desc_priv      : 2;
    uint8_t  present        : 1;
    uint8_t  limit_high     : 4;
    uint8_t  available      : 1;
    uint8_t  long_mode      : 1;
    uint8_t  op_size        : 1;
    uint8_t  granularity    : 1;
    uint8_t  base_high      : 8;

    gdt_entry() = default;
    
    gdt_entry(uint32_t base, uint32_t limit, uint8_t type) {
        limit_low = limit & 0xFFFF;
        base_low = base & 0xFFFF;
        base_middle = (base >> 16) & 0xFF;
        seg_type = type & 0xF;
        desc_type = 1;
        desc_priv = 00;
        present = 1;
        limit_high = (limit >> 16) & 0xF;
        available = 0;
        long_mode = 1;
        op_size = 1;
        granularity = 1;
        base_high = (base >> 24) & 0xFF;
    }
};
static_assert(sizeof(struct gdt_entry) == 8, "gdt_entry is not 8 bytes long!");

// Figure 2.6 from Intel SDM 2.4.1
struct [[gnu::packed]] gdt_ptr {
    uint16_t limit : 16;
    uint64_t base  : 64;
};
static_assert(sizeof(struct gdt_ptr) == 10, "gdt_ptr is not 10 bytes long!");

// Define initial GDT entries
static struct gdt_entry gdt[3] {
    gdt_entry{}, // null
    gdt_entry{0, 0xFFFFFFFF, 0b1010}, // code
    gdt_entry{0, 0xFFFFFFFF, 0b0010}, // data
};

// Pointer to GDT
static struct gdt_ptr gdt_ptr {
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

//===----------------------------------------------------------------------===//
// Main entry point (called from start.s)

extern "C" void main(int sig, unsigned long ptr) {
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

    // Reserve MBI structures so our PMM doesn't overwrite them
    reserved_memory[1].begin = module->mod_start;
    reserved_memory[1].end = module->mod_end;
    reserved_memory[2].begin = (vaddr_t) mmap;
    reserved_memory[2].end = (vaddr_t) mmap + mmap->size;

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

    // Initialize PMM
    bootstrap_pmm(mmap);

    for(;;);
}
