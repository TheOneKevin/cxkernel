#include "arch/interface.h"
#include "core/vm.h"
#include "ebl/linked_list.h"
#include "ebl/status.h"

namespace x86_64 {

   //===------------------------------------------------------------------===//
   // Kernel virtual memory map

   // FIXME: Not implemented yet

   //===------------------------------------------------------------------===//
   // Internal (non-interface) arch-specific functions

   void init_idt();
   Result<void> map_pages(ebl::IntrusiveList<core::Page>& pages, vaddr_t virt, page_flags flags);

} // namespace x86_64
