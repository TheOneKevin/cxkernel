#pragma once

#include "loaderabi.h"
#include "core/vm.h"
#include "ebl/status.h"

namespace core {
    status_t alloc_phys_pages(unsigned count, ebl::IntrusiveList<core::Page>& pages);
    status_t alloc_phys_page_single(Page*& out);
    void free_phys_pages(ebl::IntrusiveList<core::Page>& pages);
    void free_phys_page_single(Page* page);
}

namespace g {
    extern decltype(loader_state_.pfndb_freelist)& pfndb_freelist;
    extern decltype(loader_state_.pfndb_rsrvlist)& pfndb_rsrvlist;
    extern decltype(loader_state_.total_phys_pgs)& total_phys_pgs;
}
