#pragma once

#include <ebl/linked_list.h>
#include <elf/elf.h>

#include "arch/interface.h"

namespace core {
   struct ABICOMPAT Page;
}

// I love boba :)
constexpr uint32_t LOADER_ABI_MAGIC_START = 0xCAFEB0BA;
constexpr uint32_t LOADER_ABI_MAGIC_END = 0xBADBEEEF;

struct PACKED ABICOMPAT LoaderState final {
   uint32_t magic_start;
   vaddr_t kernel_elf;
   ebl::IntrusiveList<core::Page> pfndb_rsrvlist;
   ebl::IntrusiveList<core::Page> pfndb_freelist;
   paddr_t total_phys_pgs;
   union {
      core::Page* pfndb_arr;
      vaddr_t unused0_;
   };
   arch::LoaderState arch_state;
   uint32_t magic_end;
};

namespace g {
   extern LoaderState loader_state_;
}
