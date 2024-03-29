#include <ebl/assert.h>

#include "arch/types.h"
#include "core/mem.h"
#include "core/vm.h"
#include "loaderabi.h"
#include "x86-64/private.h"

using core::Page;
using x86_64::page_flags;
using page_list_head = ebl::IntrusiveList<core::Page>;

static Result<void> map_single_page(paddr_t phys,
                                    vaddr_t virt,
                                    page_flags flags,
                                    page_list_head& pt_pages) {
   namespace ns = x86_64;
   assert(phys % arch::page_size == 0, "Physical address is not page-aligned!");
   assert(virt % arch::page_size == 0, "Virtual address is not page-aligned!");

   const uint16_t pml4e_idx = ns::pml4e_index(virt);
   const uint16_t pdpte_idx = ns::pdpte_index(virt);
   const uint16_t pde_idx = ns::pde_index(virt);
   const uint16_t pte_idx = ns::pte_index(virt);

   auto* const pml4 = (ns::pml4e*)ns::virt_from_index(511, 511, 511, 511);
   auto* const pdpt = (ns::pdpte*)ns::virt_from_index(511, 511, 511, pml4e_idx);
   auto* const pd = (ns::pde*)ns::virt_from_index(511, 511, pdpte_idx, pml4e_idx);
   auto* const pt = (ns::pte*)ns::virt_from_index(511, pde_idx, pdpte_idx, pml4e_idx);

   auto* pml4e = &pml4[pml4e_idx];
   auto* pdpte = &pdpt[pdpte_idx];
   auto* pde = &pd[pde_idx];
   auto* pte = &pt[pte_idx];

   if(!pml4e->f.present) {
      auto ec = core::alloc_phys_page_single();
      if(!ec) return ec.status();
      auto page = ec.unwrap();
      pt_pages.push_back(page);
      pml4e->data = page->paddr() | 1 | flags.data;
   }

   if(!pdpte->f.present) {
      auto ec = core::alloc_phys_page_single();
      if(!ec) return ec.status();
      auto page = ec.unwrap();
      pt_pages.push_back(page);
      pdpte->data = page->paddr() | 1 | flags.data;
   }

   if(!pde->f.present) {
      auto ec = core::alloc_phys_page_single();
      if(!ec) return ec.status();
      auto page = ec.unwrap();
      pt_pages.push_back(page);
      pde->data = page->paddr() | 1 | flags.data;
   }

   // Force overwrite PTE if present
   pte->data = phys | 1 | flags.data;

   return E::OK;
}

Result<void> x86_64::map_pages(page_list_head& pages, vaddr_t virt, page_flags flags) {
   page_list_head pt_pages;
   Result<void> ec = E::OK;
   for(auto page : pages) {
      ec = map_single_page(page->paddr(), virt, flags, pt_pages);
      if(!ec) {
         // FIXME: This function does not clean up mappings properly!
         core::free_phys_pages(pt_pages);
         return ec;
      }
      virt += arch::page_size;
   }
   return E::OK;
}

/**
 * To understand recursive virtual mappings, let's manually compute the translations.
 * Given a virtual address (x, y, z, w) where each number represents an index into
 * the respective level's table, let's compute the physical address.
 *
 * Let's assign PML4[511] -> PML4.
 *
 * (x, y, z, w) maps to what paddr? (x < 511) This is just regular page table mappings.
 *     Level | Lookup
 *     ------|------------------
 *     PML4  | PML4[511] -> PDPT
 *     PDPT  | PDPT[x  ] -> PD
 *     PD    | PD  [y  ] -> PT
 *     PT    | PT  [z  ] -> PTE
 * (511, 511, 511, 511) maps to what physical address?
 *     Level | Lookup
 *     ------|------------------
 *     PML4  | PML4[511] -> PML4
 *     PDPT  | PML4[511] -> PML4
 *     PD    | PML4[511] -> PML4
 *     PT    | PML4[511] -> PML4
 * (511, 511, 511, x) maps to what paddr? (x < 511)
 *     Level | Lookup
 *     ------|------------------
 *     PML4  | PML4[511] -> PML4
 *     PDPT  | PML4[511] -> PML4
 *     PD    | PML4[511] -> PML4
 *     PT    | PML4[x  ] -> PDPT
 * (511, 511, x, y) maps to what paddr? (x < 511)
 *     Level | Lookup
 *     ------|------------------
 *     PML4  | PML4[511] -> PML4
 *     PDPT  | PML4[511] -> PML4
 *     PD    | PML4[x  ] -> PDPT
 *     PT    | PDPT[y  ] -> PD
 * (511, x, y, z) maps to what paddr? (x < 511)
 *     Level | Lookup
 *     ------|------------------
 *     PML4  | PML4[511] -> PML4
 *     PDPT  | PML4[x  ] -> PDPT
 *     PD    | PDPT[y  ] -> PD
 *     PT    | PD  [z  ] -> PT
 */
