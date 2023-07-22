#include "loader.h"
#include <ebl/util.h>

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
// Main entry point (called from start.s)

extern "C" void main(int sig, struct multiboot_header* ptr) {
    (void) sig;
    (void) ptr;
    (void) gdt_ptr;
    console_init(COM1);
    console_init(COM2);
    console_puts("Hello, world!\n");
    console_log("Hello, world! (from the logs)\n");
    for(;;);
}
