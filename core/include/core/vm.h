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
    using pfndb_node = ebl::IntrusiveListNode<core::page, 1>;
    using pfndb_head = pfndb_node::list<0>;
    static_assert(sizeof(pfndb_node) - sizeof(core::page) == sizeof(vaddr_t)*2,
        "Size of page node is unexpected given size of page struct!");
}

// Associate core::page with pfndb_node
template<> struct ebl::LinkedRef<core::page> {
    typedef core::pfndb_node type;
};
