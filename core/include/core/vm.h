#pragma once

#include "arch/types.h"
#include <stdint.h>

namespace core {

struct address_space {

};

struct page {
    uint32_t flags;
    vaddr_t virtual_addr;
    union {
        vaddr_t u3_;
        struct address_space* address_space;
    };
};

}
