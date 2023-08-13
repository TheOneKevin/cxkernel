#pragma once

#include <elf/elf.h>
#include "core/vm.h"
#include "arch/interface.h"

// I love boba :)
constexpr uint32_t LOADER_ABI_MAGIC_START = 0xCAFEB0BA;
constexpr uint32_t LOADER_ABI_MAGIC_END   = 0xBADBEEEF;
 
struct PACKED ABICOMPAT LoaderState {
    uint32_t magic_start;
    vaddr_t kernel_elf;
    core::page_list_head pfndb_rsrvlist;
    core::page_list_head pfndb_freelist;
    paddr_t total_phys_pgs;
    arch::LoaderState arch_state;
    uint32_t magic_end;
};

namespace g {
    extern LoaderState loader_state_;
}
