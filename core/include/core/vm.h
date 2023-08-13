#pragma once

#include "arch/types.h"
#include "core/spinlock.h"
#include <stdint.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {
    struct ABICOMPAT Page;
    struct VmMapping;
    struct VmRegion;
    struct AddressSpace;

    using page_list_type = ebl::IntrusiveList<Page, 1>;
    using page_list_head = page_list_type::list<0>;
    using vm_mapping_list_type = ebl::IntrusiveList<VmMapping, 1>;
    using vm_mapping_list_head = vm_mapping_list_type::list<0>;
    using vm_region_list_type = ebl::IntrusiveList<VmRegion, 1>;
    using vm_region_list_head = vm_region_list_type::list<0>;

    struct VmMapping {
        vaddr_t offset;
        vaddr_t limit;
        page_list_head pages;
    };
    struct VmRegion {
        vaddr_t base;
        vaddr_t size;
        uint32_t flags;
        struct VmRegion* parent;
        vm_region_list_head children;
        vm_mapping_list_head mappings;
    };
    struct AddressSpace {
        struct VmRegion* user_root;
        struct VmRegion* kernel_root;
        struct arch::AddressSpace backend;
    };
    struct ABICOMPAT Page {
        uint32_t flags;
        core::Spinlock lock;
        union {
            vaddr_t u3_;
            struct AddressSpace* address_space;
        };
    };
    using page_node = page_list_type::node;
    static_assert(sizeof(page_node) - sizeof(Page) == sizeof(vaddr_t)*2,
        "Size of page node is unexpected given size of page struct!");
}

// Do not associate core::page with page_node
ProhibitLinkedRef(core::Page);
