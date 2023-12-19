#include "core/vm.h"
#include "core/mem.h"
#include "x86-64/types.h"
#include <ebl/assert.h>
#include <ebl/memory.h>
#include <ebl/status.h>
#include <ebl/util.h>

using namespace core;
using ebl::RefPtr;
using ebl::MakeRefPtr;
using ebl::AllocChecker;

Result<RefPtr<VmRegion>>
VmRegion::allocate_vmr_compact(size_t size, uint8_t align_pow2, VmRegionFlags flags) {
    // Ensure child VMR capability is a subset of parent VMR capability.
    if((flags.capability & flags_.capability) != flags.capability) {
        return E::PERMISSION_DENIED;
    }
    // If the region has no children, create a hole equal to the region.
    if(children_.empty()) {
        AllocChecker ac{};
        auto hole = MakeRefPtr<VmRegion>(ac);
        if(!ac.check())
            return E::OUT_OF_MEMORY;
        hole->base_ = base_;
        hole->size_ = size_;
        hole->flags_.type = VmRegionType::HOLE;
        hole->flags_.capability = 0;
        hole->parent_ = this;
        hole->aspace_ = aspace_;
        children_.push_front_unsafe(hole);
    }
    // TODO: Currently, this is just a first-fit allocator.
    const vaddr_t align = ebl::max(1U << align_pow2, arch::page_size);
    assert(size % arch::page_size == 0, "Size is not page-aligned!");
    // TODO: This is a linear search. We should use a tree or something?
    for(auto child : children_) {
        if(child->flags_.type != VmRegionType::HOLE)
            continue;
        auto aligned_base = ebl::align_up(child->base_, align);
        if(aligned_base + size > child->base_ + child->size_)
            continue;
        auto ec = child->split(aligned_base - child->base_, size);
        if(!ec) return ec.status();
        auto vmr_out = ec.unwrap();
        vmr_out->flags_.capability = flags.capability;
        return vmr_out;
    }
    return E::OUT_OF_MEMORY;
}

Result<RefPtr<VmRegion>>
VmRegion::allocate_vmr_sparse(size_t size, uint8_t align_pow2, VmRegionFlags flags) {
    (void) size;
    (void) align_pow2;
    (void) flags;
    return E::NOT_IMPLEMENTED;
}

Result<RefPtr<VmRegion>>
VmRegion::map_pages(vaddr_t offset, size_t size, VmRegionFlags flags,
                    RefPtr<VmObject> object, vaddr_t vmo_offset,
                    arch::mmu_flags mmu_flags) {
    (void) offset;
    (void) size;
    (void) flags;
    (void) mmu_flags;
    (void) object;
    (void) vmo_offset;
    return E::NOT_IMPLEMENTED;
}

Result<void>
VmRegion::protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags) {
    (void) addr;
    (void) size;
    (void) flags;
    return E::NOT_IMPLEMENTED;
}

Result<RefPtr<VmRegion>> VmRegion::split(vaddr_t offset, size_t size) {
    /**
     * To split a continuous hole, we perform 2 cuts like so.
     * Before split:
     * | region                  |
     *      ^start_addr   ^end_addr
     * 
     * After split:
     * | A  | B           | C    |
     *      ^cut1         ^cut2
     * Where we take A as the original hole (this) and B as the
     * allocated region and C as a new hole.
     */
    const vaddr_t start_addr = base_ + offset;
    const vaddr_t end_addr = start_addr + size;
    const bool docut1 = start_addr != base_;
    const bool docut2 = end_addr != base_ + size_;
    assert(this->flags_.type == VmRegionType::HOLE, "Cannot split non-hole region!");

    RefPtr<VmRegion> new_hole{}, new_vmr{};
    AllocChecker ac{};

    if(docut1) {
        new_vmr = MakeRefPtr<VmRegion>(ac);
        if(!ac.check())
            return E::OUT_OF_MEMORY;
        new_vmr->base_ = start_addr;
        new_vmr->size_ = size;
        new_vmr->flags_.type = VmRegionType::REGION;
        new_vmr->parent_ = this->parent_;
        new_vmr->aspace_ = this->aspace_;
    }

    if(docut2) {
        new_hole = MakeRefPtr<VmRegion>(ac);
        if(!ac.check())
            return E::OUT_OF_MEMORY;
        new_hole->base_ = end_addr;
        new_hole->size_ = size_ - size - offset;
        new_hole->flags_.type = VmRegionType::HOLE;
        new_hole->flags_.capability = 0;
        new_hole->parent_ = this->parent_;
        new_hole->aspace_ = this->aspace_;
    }

    if(docut1 && docut2) {
        this->size_ = offset;
        this->insert_after(new_vmr);
        new_vmr->insert_after(new_hole);
    } else if(docut1 && !docut2) {
        this->size_ = offset;
        this->insert_after(new_vmr);
    } else if(!docut1 && docut2) {
        this->size_ = size;
        this->insert_after(new_hole);
        new_vmr = ebl::AdoptRef(this);
    } else {
        new_vmr = ebl::AdoptRef(this);
        this->flags_.type = VmRegionType::REGION;
    }

    return new_vmr;
}
