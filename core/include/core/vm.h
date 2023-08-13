#pragma once

#include "arch/types.h"
#include "core/spinlock.h"
#include <stdint.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {
    struct ABICOMPAT page;
    struct vm_mapping;
    struct vm_region;
    struct address_space;

    using page_list_type = ebl::IntrusiveList<page, 1>;
    using page_list_head = page_list_type::list<0>;
    using vm_mapping_list_type = ebl::IntrusiveList<vm_mapping, 1>;
    using vm_mapping_list_head = vm_mapping_list_type::list<0>;
    using vm_region_list_type = ebl::IntrusiveList<vm_region, 1>;
    using vm_region_list_head = vm_region_list_type::list<0>;

    struct vm_mapping {
        vaddr_t offset;
        vaddr_t limit;
        page_list_head pages;
    };
    struct vm_region {
        vaddr_t base;
        vaddr_t size;
        uint32_t flags;
        struct vm_region* parent;
        vm_region_list_head children;
        vm_mapping_list_head mappings;
    };
    struct address_space {
        struct vm_region* user_root;
        struct vm_region* kernel_root;
        struct arch::address_space backend;
    };
    struct ABICOMPAT page {
        uint32_t flags;
        core::Spinlock lock;
        union {
            vaddr_t u3_;
            struct address_space* address_space;
        };
    };
    using page_node = page_list_type::node;
    static_assert(sizeof(page_node) - sizeof(page) == sizeof(vaddr_t)*2,
        "Size of page node is unexpected given size of page struct!");
}

// Do not associate core::page with page_node
ProhibitLinkedRef(core::page);
