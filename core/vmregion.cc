#include "core/vmregion.h"

#include <ebl/assert.h>
#include <ebl/memory.h>
#include <ebl/status.h>
#include <ebl/util.h>

#include "core/mem.h"
#include "core/vm.h"
#include "ebl/type_traits.h"
#include "x86-64/types.h"

using namespace core;
using ebl::AllocChecker;
using ebl::MakeRefPtr;
using ebl::RefPtr;

Result<RefPtr<VmRegion>> VmRegion::allocate_vmr_compact(size_t size,
                                                        uint8_t align_pow2,
                                                        VmRegionFlags flags) {
   // Ensure child VMR capability is a subset of parent VMR capability.
   if((flags.capability & flags_.capability) != flags.capability) {
      return E::PERMISSION_DENIED;
   }

   // Ensure this is a region VMR.
   if(flags.type != VmRegionType::REGION) {
      return E::INVALID_ARGUMENT;
   }

   bool found_hole = false;
   vaddr_t new_hole_base;
   size_t new_hole_size;

   // The child VMR to insert before, to keep sorted order of children_
   RefPtr<VmRegion> child = nullptr;

   // TODO: Currently, this is just a first-fit allocator.
   const vaddr_t align = ebl::max(1U << align_pow2, arch::page_size);
   assert(size % arch::page_size == 0, "Size is not page-aligned!");
   foreach_hole([&](RefPtr<VmRegion> x, vaddr_t hole_base, size_t hole_size) -> bool {
      auto aligned_base = ebl::align_up(hole_base, align);
      if(aligned_base + size > hole_base + hole_size) return true;
      // Found it.
      new_hole_base = aligned_base;
      new_hole_size = size;
      found_hole = true;
      child = ebl::move(x);
      return false;
   });

   if(!found_hole) return E::ALLOCATION_FAILED;

   // Allocate the new VMR and insert it into the children_ list
   VmRegionFlags new_flags{};
   new_flags.type = VmRegionType::REGION;
   new_flags.capability = flags.capability;
   new_flags.is_root = 0;
   AllocChecker ac;
   auto new_vmr = MakeRefPtr<VmRegion>(ac, new_hole_base, new_hole_size, new_flags, aspace_);
   if(!ac.check()) return E::OUT_OF_MEMORY;
   new_vmr->parent_ = this;
   if(child == nullptr) {
      children_.push_back_unsafe(new_vmr);
   } else {
      new_vmr->insert_before(child);
   }
   return new_vmr;
}

Result<RefPtr<VmRegion>> VmRegion::allocate_vmr_sparse(size_t size,
                                                       uint8_t align_pow2,
                                                       VmRegionFlags flags) {
   (void)size;
   (void)align_pow2;
   (void)flags;
   return E::NOT_IMPLEMENTED;
}

Result<RefPtr<VmRegion>> VmRegion::map_pages(vaddr_t offset,
                                             size_t size,
                                             VmRegionFlags flags,
                                             RefPtr<VmObject> object,
                                             vaddr_t vmo_offset,
                                             arch::mmu_flags mmu_flags) {
   (void)offset;
   (void)size;
   (void)flags;
   (void)mmu_flags;
   (void)object;
   (void)vmo_offset;
   return E::NOT_IMPLEMENTED;
}

Result<void> VmRegion::protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags) {
   (void)addr;
   (void)size;
   (void)flags;
   return E::NOT_IMPLEMENTED;
}

Result<void> VmRegion::destroy() { return E::NOT_IMPLEMENTED; }

RefPtr<VmRegion> VmRegion::find_child_above(vaddr_t offset) {
   for(auto child : children_) {
      if(child->base_ >= this->base_ + offset) return child;
   }
   return nullptr;
}

bool VmRegion::does_overlap_child(vaddr_t offset, size_t size) {
   auto end = this->base_ + offset + size;
   auto child = find_child_above(offset);
   if(!child) return false;
   return child->base_ <= end;
}
