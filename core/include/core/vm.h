#pragma once

#include "arch/types.h"
#include <stdint.h>
#include <ebl/linked_list.h>

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

using pfndb_t = ebl::IntrusiveMultilist<1, core::page>;
using pfndb_list_t = pfndb_t::list<0>;
using pfndb_node_t = pfndb_t::list_node;

static_assert(sizeof(pfndb_node_t) - sizeof(core::page) == sizeof(vaddr_t)*2,
    "Size of page node is unexpected given size of page struct!");

}
