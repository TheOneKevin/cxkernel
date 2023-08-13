#include "core/vm.h"
#include "x86-64/private.h"
#include "arch/types.h"
#include "assert.h"
#include "loaderabi.h"

using x86_64::page_flags;

static void map_single_page(paddr_t phys, vaddr_t virt, page_flags flags) {
    
}

void x86_64::map_pages(core::page_node* pages, vaddr_t virt, page_flags flags) {

}
