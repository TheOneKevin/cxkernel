#include "loader.h"
#include "assert.h"
#include "core/vm.h"
#include <ebl/util.h>
#include <ebl/stdio.h>
#include <ebl/string.h>
#include <ebl/linked_list.h>

extern "C" int loader_begin_;
extern "C" int loader_end_;

constexpr int num_reserved_ranges = 4;
static range reserved_memory[num_reserved_ranges] {
    { 0, arch::page_size-1 }, // Null
    { (uint64_t) &loader_begin_, (uint64_t) &loader_end_ - 1 }, // Loader
    { 0, 0 }, // MMAP structs
    { 0, 0 } // Other ranges
};

// Bump-allocation iterator
static multiboot_mmap_list mmap_list{nullptr};
static auto mmap_it = mmap_list.begin();

// List-based PMM structs (free list and reserved list)
decltype(pfndb_freelist) pfndb_freelist{};
decltype(pfndb_rsrvlist) pfndb_rsrvlist{};
core::Page* pfndb_arr = nullptr;
paddr_t total_phys_pgs = 0;
paddr_t pfndb_sz_bytes = 0;
paddr_t pfndb_sz_pgs = 0;

//===----------------------------------------------------------------------===//
// Pre-PMM early alloc functions

// Check if x in [r.begin, r.end)
constexpr bool is_in(uint32_t ptr, range& r) {
    return ptr >= r.begin && ptr < r.end;
}

// Check if a page intersects a reserved range (see reserved_memory[])
static bool is_page_reserved(uint64_t page) {
    for(int i = 0; i < num_reserved_ranges; i++) {
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
static paddr_t early_pmm_alloc_cts(uint64_t pages, int slimit = 10) {
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
    return early_pmm_alloc_cts(pages, slimit-1);
}

//===----------------------------------------------------------------------===//
// bootstrap_pmm

void bootstrap_pmm(const range (&res)[1], struct multiboot_tag_mmap *mmap) {
    // Initialize bump allocator
    reserved_memory[2].begin = (vaddr_t) mmap;
    reserved_memory[2].end = (vaddr_t) mmap + mmap->size - 1;
    reserved_memory[3] = res[0];
    mmap_list = multiboot_mmap_list {mmap};
    mmap_it = mmap_list.begin();

    // Print reserved ranges
    ebl::klog("Reserved ranges:\n");
    for(int i = 0; i < num_reserved_ranges; i++) {
        ebl::klog("  %016lx - %016lx\n", reserved_memory[i].begin, reserved_memory[i].end);
    }
    
    // Get largest phys addr
    paddr_t largest_phys_addr = 0;
    for(auto* entry : mmap_list) {
        if(entry->type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;
        largest_phys_addr = ebl::max(largest_phys_addr, (paddr_t)(entry->addr + entry->len));
    }
    largest_phys_addr = arch::page_align_down(largest_phys_addr);

    // Allocate PFN database
    total_phys_pgs = largest_phys_addr / arch::page_size;
    pfndb_sz_bytes = total_phys_pgs * sizeof(core::Page);
    pfndb_sz_pgs = arch::page_align_up(pfndb_sz_bytes) / arch::page_size;
    pfndb_arr = (core::Page*) early_pmm_alloc_cts(pfndb_sz_pgs);
    ebl::memset((void*) pfndb_arr, 0, pfndb_sz_bytes);

    // Preserve PFN database while disposing of mmap structs
    reserved_memory[2].begin = (paddr_t) pfndb_arr;
    reserved_memory[2].end = (paddr_t) pfndb_arr + pfndb_sz_bytes;

    // Initialize PFN database
    for(paddr_t i = 0; i < total_phys_pgs; i++) {
        if(is_page_reserved(i)) {
            pfndb_rsrvlist.push_back_unsafe(&pfndb_arr[i]);
        } else {
            pfndb_freelist.push_back_unsafe(&pfndb_arr[i]);
        }
    }

    // Verify list sizes
    ebl::klog("PFN DB size: %lu free + %lu used = %lu total\n",
        (uint64_t) pfndb_freelist.size(), (uint64_t) pfndb_rsrvlist.size(),
        (uint64_t) pfndb_freelist.size() + pfndb_rsrvlist.size());
    ebl::klog("PFN DB addr: %lx - %lx\n",
        (paddr_t) pfndb_arr, (paddr_t) pfndb_arr+pfndb_sz_bytes-1);
    assert(pfndb_freelist.size() + pfndb_rsrvlist.size() == total_phys_pgs,
        "PFN database list sizes are incorrect!");
}

//===----------------------------------------------------------------------===//
// Enable paging

static ns::pml4e* pml4;

paddr_t pmm_alloc_page() {
    auto* node = pfndb_freelist.pop_front_unsafe();
    if(node == nullptr)
        assert(false, "Out of memory!\n");
    pfndb_rsrvlist.push_back_unsafe(node);
    // Get i such that pfndb_arr[i] == node
    const paddr_t i = node - pfndb_arr;
    assert(&pfndb_arr[i] == node, "PFN database is corrupted!");
    // Reset page
    auto page = i * arch::page_size;
    ebl::memset((void*) page, 0, arch::page_size);
    return page;
}

void map_page(paddr_t phys, vaddr_t virt, ns::page_flags flags) {
    assert(phys % arch::page_size == 0, "Physical address is not page-aligned!");
    assert(virt % arch::page_size == 0, "Virtual address is not page-aligned!");
    // ebl::klog("Mapping phys -> virt: %lx -> %lx\n", phys, virt);
    auto* pml4e = &pml4[ns::pml4e_index(virt)];
    if(!pml4e->f.present) {
        pml4e->data = pmm_alloc_page() | 1 | flags.data;
    }
    auto* pdpte = &((ns::pdpte*)(pml4e->data & 0xFFFFF800))[ns::pdpte_index(virt)];
    if(!pdpte->f.present) {
        pdpte->data = pmm_alloc_page() | 1 | flags.data;
    }
    auto* pde = &((ns::pde*)(pdpte->data & 0xFFFFF800))[ns::pde_index(virt)];
    if(!pde->f.present) {
        pde->data = pmm_alloc_page() | 1 | flags.data;
    }
    auto* pte = &(((ns::pte*)(pde->data & 0xFFFFF800))[ns::pte_index(virt)]);
    if(!pte->f.present) {
        pte->data = phys | 1 | flags.data;
    }
}

uint64_t setup_paging() {
    pml4 = (ns::pml4e*) pmm_alloc_page();
    ebl::memset(pml4, 0, arch::page_size);
    // Self-referencing entry
    pml4[511].data = (uint64_t) pml4 | ns::page_flags{
        .f {
            .present = 1,
            .writable = 1,
            .user = 0
        }
    }.data;
    return (uint64_t) pml4;
}
