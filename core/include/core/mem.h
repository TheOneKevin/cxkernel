#pragma once

#include "core/vm.h"
#include "ebl/status.h"
#include "loaderabi.h"

namespace core {
   Result<void> alloc_phys_pages(unsigned count, ebl::IntrusiveList<core::Page>& pages);
   Result<Page*> alloc_phys_page_single();
   void free_phys_pages(ebl::IntrusiveList<core::Page>& pages);
   void free_phys_page_single(Page* page);
} // namespace core

namespace g {
   extern decltype(loader_state_.pfndb_freelist)& pfndb_freelist;
   extern decltype(loader_state_.pfndb_rsrvlist)& pfndb_rsrvlist;
   extern decltype(loader_state_.total_phys_pgs)& total_phys_pgs;
} // namespace g
