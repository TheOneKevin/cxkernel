// NOTE: System V ABI gaurentees bitfield order in packed structs,
//       even though the C++ standard does not.

#pragma once

#include <stdint.h>

typedef uint64_t vaddr_t;
typedef uint64_t paddr_t;
typedef uint64_t timestamp_t;

namespace arch {
    constexpr unsigned int page_size = 4096;
}

namespace x86_64 {

    //===------------------------------------------------------------------===//
    // Paging helper functions

    // Canonicalizes a virtual address
    constexpr vaddr_t canonicalize(vaddr_t addr) {
        static_assert(sizeof(vaddr_t) == sizeof(int64_t));
        return ((int64_t) addr << 16) >> 16;
    }

    // Gets the physical address from a PML4E, PDPTE, PDE, and PTE index
    constexpr uint64_t phys_from_index(
        uint16_t pml4e_index,
        uint16_t pdpte_index,
        uint16_t pde_index,
        uint16_t pte_index
    ) {
        return canonicalize(
            ((uint64_t) (pml4e_index & 0x1FF) << 39) |
            ((uint64_t) (pdpte_index & 0x1FF) << 30) |
            ((uint64_t) (pde_index & 0x1FF) << 21) |
            ((uint64_t) (pte_index & 0x1FF) << 12));
    }

    // Gets the index of the PML4 entry for a virtual address
    // ref: Figure 4-8 from Intel SDM Vol 3 4.5.4
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

    //===------------------------------------------------------------------===//
    // CPU structures

    // ref: Figure 4-11 from Intel SDM Vol 3 4.5.4
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

    // ref: Table 3-1 from Intel SDM Vol 3 3.4.5.1
    enum class segment_type : uint8_t {
        DATA_READ = 0b0000,
        DATA_READ_WRITE = 0b0010,
        CODE_EXECUTE_ACCESSED = 0b1001,
        CODE_EXECUTE_READ = 0b1010
    };

    // ref: Table 3-2 from Intel SDM Vol 3 3.5
    enum class descriptor_type : uint8_t {
        TASK_GATE = 0b0101,
        INTERRUPT_GATE = 0b1110,
        TRAP_GATE = 0b1111,
    };

    // ref: Figure 3-8 from Intel SDM Vol 3 3.4.5
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
        
        gdt_entry(uint32_t base, uint32_t limit, segment_type type) {
            limit_low = limit & 0xFFFF;
            base_low = base & 0xFFFF;
            base_middle = (base >> 16) & 0xFF;
            seg_type = (uint8_t) type & 0xF;
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

    // ref: Figure 2-6 from Intel SDM Vol 3 2.4.1
    struct PACKED gdt_ptr {
        uint16_t limit : 16;
        uint64_t base  : 64;
    };
    static_assert(sizeof(struct gdt_ptr) == 10, "gdt_ptr is not 10 bytes long!");

    // ref: Figure 6-8 from Intel SDM Vol 3 6.14.1
    struct PACKED idt_entry {
        uint16_t offset_low  : 16;
        uint16_t selector    : 16;
        uint8_t  ist         : 3;
        uint8_t  reserved0   : 5;
        uint8_t  type        : 4;
        uint8_t  zero        : 1;
        uint8_t  dpl         : 2;
        uint8_t  present     : 1;
        uint16_t offset_mid  : 16;
        uint32_t offset_high : 32;
        uint32_t reserved1   : 32;
        idt_entry() = default;
        idt_entry(uint64_t isr_fn, descriptor_type type) {
            offset_low = isr_fn & 0xFFFF;
            selector = 0x08;
            ist = 0;
            reserved0 = 0;
            this->type = (uint8_t) type & 0xF;
            zero = 0;
            dpl = 0;
            present = 1;
            offset_mid = (isr_fn >> 16) & 0xFFFF;
            offset_high = (isr_fn >> 32) & 0xFFFFFFFF;
            reserved1 = 0;
        }
    };
    static_assert(sizeof(struct idt_entry) == 16, "idt_entry is not 16 bytes long!");

    // ref: Figure 6-1 from Intel SDM Vol 3 6.10
    struct PACKED idt_ptr {
        uint16_t limit : 16;
        uint64_t base  : 64;
    };
    static_assert(sizeof(struct idt_ptr) == 10, "idt_ptr is not 6 bytes long!");

    struct PACKED int_frame {
        uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;         // pushed by isr_handler
        uint64_t r8, r9, r10, r11, r12, r13, r14, r15;      // pushed by isr_handler
        uint64_t vector;                                    // pushed by isrN
        uint64_t err_code;                                  // pushed by isrN
        uint64_t rip, cs, flags;                            // pushed by cpu
        uint64_t user_sp, user_ss;                          // pushed by cpu if CPL changes
    };
}
