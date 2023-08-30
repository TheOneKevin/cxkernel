#include "core/vm.h"
#include "core/mem.h"
#include "x86-64/types.h"
#include "assert.h"
#include <ebl/memory.h>
#include <ebl/status.h>
#include <ebl/util.h>

using namespace core;
using ebl::RefPtr;
using ebl::MakeRefPtr;

status_t VmRegion::allocate_vmr_compact(size_t size, uint8_t align_pow2,
                                        VmRegionFlags flags, RefPtr<VmRegion> &vmr_out) {
    // Ensure child VMR capability is a subset of parent VMR capability.
    if((flags.capability & flags_.capability) != flags.capability) {
        return E::PERMISSION_DENIED;
    }
    // TODO: Currently, this is just a first-fit allocator.
    const vaddr_t align = ebl::max(1U << align_pow2, arch::page_size);
    assert(size % arch::page_size == 0, "Size is not page-aligned!");
    // TODO: This is a linear search. We should use a tree or something?
    for(auto& child : children_) {
        if(child.flags_.type != VmRegionType::HOLE)
            continue;
        auto aligned_base = ebl::align_up(child.base_, align);
        if(aligned_base + size > child.base_ + child.size_)
            continue;
        auto ec = child.split(aligned_base - child.base_, size, vmr_out);
        if(ec != E::OK)
            return ec;
        
        return E::OK;
    }
    return E::OUT_OF_MEMORY;
}

status_t VmRegion::allocate_vmr_sparse(size_t size, uint8_t align_pow2,
                                       VmRegionFlags flags, RefPtr<VmRegion> &vmr_out) {
    (void) size;
    (void) align_pow2;
    (void) vmr_out;
    (void) flags;
    return E::NOT_IMPLEMENTED;
}

status_t VmRegion::map_pages(vaddr_t offset, size_t size, VmRegionFlags flags,
                             RefPtr<VmObject> object, vaddr_t vmo_offset,
                             arch::mmu_flags mmu_flags, RefPtr<VmRegion> &map_out) {
    (void) offset;
    (void) size;
    (void) flags;
    (void) mmu_flags;
    (void) object;
    (void) vmo_offset;
    (void) map_out;
    return E::NOT_IMPLEMENTED;
}

status_t VmRegion::protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags) {
    (void) addr;
    (void) size;
    (void) flags;
    return E::NOT_IMPLEMENTED;
}

status_t VmRegion::split(vaddr_t offset, size_t size, RefPtr<VmRegion> &vmr_out) {
    (void) offset;
    (void) size;
    (void) vmr_out;
    return E::NOT_IMPLEMENTED;
}
