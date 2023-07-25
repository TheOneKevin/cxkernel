// NOTE: System V ABI gaurentees bitfield order in packed structs,
//       even though the C++ standard does not.

#pragma once

#include <stdint.h>

typedef uint64_t vaddr_t;
typedef uint64_t paddr_t;

namespace arch {
    constexpr unsigned int page_size = 4096;
}

namespace x86_64 {
    // Gets the index of the PML4 entry for a virtual address
    // ref: Figure 4-8 from Intel SDM 4.5.4
    constexpr uint16_t pml4e_index(vaddr_t addr) {
        return (addr >> 39) & 0x1FF;
    }
    // ref: pml4e_index
    constexpr uint16_t pdpte_index(vaddr_t addr) {
        return (addr >> 30) & 0x1FF;
    }
    // ref: pml4e_index
    constexpr uint16_t pde_index(vaddr_t addr) {
        return (addr >> 21) & 0x1FF;
    }
    // ref: pml4e_index
    constexpr uint16_t pte_index(vaddr_t addr) {
        return (addr >> 12) & 0x1FF;
    }

    // ref: Figure 4-11 from Intel SDM 4.5.4
    struct PACKED generic_page_entry_raw {
        uint8_t present : 1;
        uint8_t writable : 1;
        uint8_t user : 1;
        uint8_t write_through : 1;
        uint8_t cache_disable : 1;
        uint8_t accessed : 1;
        uint8_t dirty : 1;
        uint8_t pat : 1;
        uint8_t global : 1;
        uint8_t ignored0 : 3; // Bits 9-11
        uint64_t addr : 40;
        uint8_t ignored1 : 7;
        uint8_t protection_key : 4;
        uint8_t execute_disable : 1;
    };

    union generic_page_entry {
        uint64_t data;
        generic_page_entry_raw f;
    };

    using page_flags = generic_page_entry;
    using pml4e = union generic_page_entry;
    using pdpte = union generic_page_entry;
    using pde   = union generic_page_entry;
    using pte   = union generic_page_entry;
    static_assert(sizeof(pml4e) == 8, "Size of pml4e is incorrect!");
    static_assert(sizeof(pdpte) == 8, "Size of pdpte is incorrect!");
    static_assert(sizeof(pde) == 8, "Size of pde is incorrect!");
    static_assert(sizeof(pte) == 8, "Size of pte is incorrect!");

    // ref: Figure 3-8 from Intel SDM 3.4.5
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
            op_size = 0;
            granularity = 0;
            base_high = (base >> 24) & 0xFF;
        }
    };
    static_assert(sizeof(struct gdt_entry) == 8, "gdt_entry is not 8 bytes long!");

    // ref: Figure 2-6 from Intel SDM 2.4.1
    struct PACKED gdt_ptr {
        uint16_t limit : 16;
        uint64_t base  : 64;
    };
    static_assert(sizeof(struct gdt_ptr) == 10, "gdt_ptr is not 10 bytes long!");
}
