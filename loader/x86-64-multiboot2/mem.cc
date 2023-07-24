#include "loader.h"
#include "assert.h"
#include "core/vm.h"
#include <ebl/util.h>
#include <ebl/stdio.h>
#include <ebl/string.h>
#include <ebl/linked_list.h>

extern "C" int loader_begin_;
extern "C" int loader_end_;

range reserved_memory[num_reserved_ranges] {
    { (uint64_t) &loader_begin_, (uint64_t) &loader_end_ },
    { 0, 0 }, // Kernel
    { 0, 0 }  // MMAP structs
};

static multiboot_mmap_list mmap_list{nullptr};
static auto mmap_it = mmap_list.begin();

static ebl::IntrusiveMultilist<1, core::page> pfndb_mlist{};
static decltype(pfndb_mlist)::list<0> pfndb_list{};
using page_node_t = decltype(pfndb_mlist)::list_node;
static_assert(sizeof(page_node_t) - sizeof(core::page) == sizeof(vaddr_t)*2,
    "Size of page node is unexpected given size of page struct!");

//===----------------------------------------------------------------------===//
// Allocation functions

// Check if x in [r.begin, r.end)
constexpr bool is_in(uint32_t ptr, range& r) {
    return ptr >= r.begin && ptr < r.end;
}

// Check if a page intersects a reserved range (see reserved_memory[])
static bool is_page_reserved(uint64_t page) {
    for(int i = 0; i < num_reserved_ranges; ++i) {
        if(
            is_in(page*arch::page_size, reserved_memory[i]) ||
            is_in((page+1)*arch::page_size, reserved_memory[i])
        ) return true;
    }
    return false;
}

// Search for a cts run of "pages" pages in the range [base, limit)
static bool search_run(uint64_t pages, paddr_t base, paddr_t limit, paddr_t& result) {
    base /= arch::page_size;
    limit /= arch::page_size;
    for(uint64_t ptr = base; ptr < limit - pages; ptr++) {
        bool has_run = true;
        for(uint64_t i = ptr; i < ptr + pages; i++) {
            if(is_page_reserved(i)) {
                has_run = false;
                break;
            }
        }
        if(has_run) {
            result = ptr * arch::page_size;
            return true;
        }
    }
    return false;
}

// Allocate a contiguous run of pages, attempt at most slimit times
static paddr_t pmm_allocate_cts(uint64_t pages, int slimit = 10) {
    assert(slimit != 0, "Out of memory, stack depth exceeded!\n");
    assert(mmap_it != mmap_list.end(), "Out of memory!\n");
    while((*mmap_it)->type != MULTIBOOT_MEMORY_AVAILABLE) {
        ++mmap_it;
        assert(mmap_it != mmap_list.end(), "Out of memory!\n");
    }
    auto* entry = *mmap_it;
    const paddr_t page_bas = arch::page_align_up(entry->addr);
    const paddr_t page_lim = arch::page_align_down(entry->addr + entry->len);
    paddr_t result;
    if(search_run(pages, page_bas, page_lim, result))
        return result;
    ++mmap_it;
    return pmm_allocate_cts(pages, slimit-1);
}

//===----------------------------------------------------------------------===//
// bootstrap_pmm

void bootstrap_pmm(struct multiboot_tag_mmap *mmap) {
    mmap_list = multiboot_mmap_list {mmap};
    mmap_it = mmap_list.begin();
    
    // Get largest phys addr
    paddr_t largest_phys_addr = 0;
    for(auto* entry : mmap_list) {
        if(entry->type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;
        largest_phys_addr = ebl::max(largest_phys_addr, (paddr_t)(entry->addr + entry->len));
    }
    largest_phys_addr = arch::page_align_down(largest_phys_addr);

    // Allocate PFN database
    const paddr_t total_phys_pgs = largest_phys_addr / arch::page_size;
    const paddr_t pfndb_sz_bytes = total_phys_pgs * sizeof(core::page);
    const paddr_t pfndb_sz_pgs = arch::page_align_up(pfndb_sz_bytes) / arch::page_size;
    const paddr_t pfndb_base = pmm_allocate_cts(pfndb_sz_pgs);
    ebl::kout("PFN database: %016lx - %016lx\n", pfndb_base, pfndb_base+pfndb_sz_bytes-1);
    ebl::memset((void*) pfndb_base, 0, pfndb_sz_bytes);

    // Preserve PFN database while disposing of mmap structs
    reserved_memory[2].begin = pfndb_base;
    reserved_memory[2].end = pfndb_base + pfndb_sz_pgs*arch::page_size;

}
