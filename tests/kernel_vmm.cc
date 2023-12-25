#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "arch/interface.h"
#include "core/vmregion.h"
#include "ebl/memory.h"
#include "ebl/status.h"
#include "x86-64/types.h"
#include <iostream>
#include "core/vm.h"
#include "doctest.h"

using core::VmRegion;
using ebl::RefPtr;

namespace arch {
   void init_aspace(core::AddressSpace& aspace) {
      aspace.user_root_.base_ = 0;
      aspace.user_root_.size_ = 6 * arch::page_size;
   }
} // namespace arch

void assert(bool cond, char const* msg) { CHECK(cond); }

static int basic_allocate_and_check(core::AddressSpace& as,
                                    int N,
                                    std::tuple<E, vaddr_t, vaddr_t> allocs[]) {
   // Allocate the regions
   std::vector<RefPtr<VmRegion>> results;
   for(int i = 0; i < N; i++) {
      auto alloc = allocs[i];
      auto res = as.get_user_root().allocate_vmr_compact(std::get<2>(alloc), 0, {});
      CHECK_MESSAGE(res.status() == std::get<0>(alloc), "Checking allocation status");
      if(res.status() == E::OK) {
         results.push_back(res.unwrap());
      }
   }
   // Check that the allocations succeeded
   int i = 0;
   for(auto child : as.get_user_root()) {
      CHECK_MESSAGE(child->get_base() == std::get<1>(allocs[i]),
                    "Checking VMR allocation base address");
      CHECK_MESSAGE(child->get_size() == std::get<2>(allocs[i]), "Checking VMR allocation size");
      CHECK_MESSAGE(child == results[i].get(), "Checking VMR allocation result");
      i++;
   }
   return i;
}

TEST_CASE("basic vmm allocation") {
   auto as = core::AddressSpace{};
   arch::init_aspace(as);
   // (base, size) pairs for allocations
   std::tuple<E, vaddr_t, vaddr_t> allocs[4] = {
         {E::OK, 0 * arch::page_size, 1 * arch::page_size},
         {E::OK, 1 * arch::page_size, 1 * arch::page_size},
         {E::OK, 2 * arch::page_size, 1 * arch::page_size},
   };
   // Allocate the regions
   int i = basic_allocate_and_check(as, 3, allocs);
   CHECK(i == 3);
   CHECK(static_cast<bool>(as.get_user_root().destroy()));
}

TEST_CASE("basic out-of-memory") {
   auto as = core::AddressSpace{};
   arch::init_aspace(as);
   // (base, size) pairs for allocations
   std::tuple<E, vaddr_t, vaddr_t> allocs[4] = {
         {E::OK, 0 * arch::page_size, 1 * arch::page_size},
         {E::OK, 1 * arch::page_size, 5 * arch::page_size},
         {E::ALLOCATION_FAILED, 0, 1 * arch::page_size},
         {E::ALLOCATION_FAILED, 0, 1 * arch::page_size},
   };
   // Allocate the regions
   int i = basic_allocate_and_check(as, 4, allocs);
   CHECK(i == 2);
   CHECK(static_cast<bool>(as.get_user_root().destroy()));
}
