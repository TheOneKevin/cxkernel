#include <ebl/linked_list.h>

#include "core/mem.h"
#include "core/vm.h"

namespace g {
   auto& pfndb_freelist = loader_state_.pfndb_freelist;
   auto& pfndb_rsrvlist = loader_state_.pfndb_rsrvlist;
   auto& total_phys_pgs = loader_state_.total_phys_pgs;
} // namespace g

Result<core::Page*> core::alloc_phys_page_single() {
   auto* node = g::pfndb_freelist.pop_front();
   if(node == nullptr) return E::OUT_OF_MEMORY;
   return node;
}

Result<void> core::alloc_phys_pages(unsigned count, ebl::IntrusiveList<Page>& pages) {
   pages = {};
   for(unsigned i = 0; i < count; i++) {
      auto* node = g::pfndb_freelist.pop_front();
      if(node == nullptr) {
         while(!pages.empty()) {
            g::pfndb_freelist.push_front(pages.pop_front());
         }
         return E::OUT_OF_MEMORY;
      }
      pages.push_front(node);
   }
   return E::OK;
}

void core::free_phys_pages(ebl::IntrusiveList<Page>& pages) {
   while(!pages.empty()) {
      auto* page = pages.pop_front();
      g::pfndb_freelist.push_back(page);
   }
}

void core::free_phys_page_single(Page* page) {
   // FIXME: Add more sophisticated checks
   if(page != nullptr) {
      g::pfndb_freelist.push_back(page);
   }
}
