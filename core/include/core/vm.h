#pragma once

#include "arch/types.h"
#include <stdint.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {
    struct address_space {

    };
    struct ABICOMPAT page {
        uint32_t flags;
        vaddr_t virtual_addr;
        union {
            vaddr_t u3_;
            struct address_space* address_space;
        };
    };
    using page_node = ebl::IntrusiveListNode<core::page, 1>;
    using pfndb_head = page_node::list<0>;
    static_assert(sizeof(page_node) - sizeof(core::page) == sizeof(vaddr_t)*2,
        "Size of page node is unexpected given size of page struct!");
}

// Do not associate core::page with page_node
ProhibitLinkedRef(core::page);
