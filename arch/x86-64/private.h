#include "core/vm.h"
#include "arch/interface.h"

namespace x86_64 {

    //===------------------------------------------------------------------===//
    // Kernel virtual memory map

    // FIXME: Not implemented yet

    //===------------------------------------------------------------------===//
    // Internal (non-interface) arch-specific functions

    void init_idt();
    void map_pages(core::page_node* pages, vaddr_t virt, page_flags flags);

} // namespace x86_64
