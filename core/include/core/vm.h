#pragma once

#include "ebl/bit.h"
#include "loaderabi.h"
#include "arch/types.h"
#include "core/spinlock.h"
#include <stdint.h>
#include <ebl/status.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {
    struct ABICOMPAT Page;
    struct VmObject;
    struct VmRegion;
    struct AddressSpace;

    struct VmObject final : ebl::RefCountable<VmObject*> {
    private:
        core::Spinlock lock_;
        ebl::IntrusiveList<Page> pages_;
    };

    enum class VmRegionType : uint8_t {
        HOLE = 0,
        MAPPING = 1,
        REGION = 2
    };

    union VmRegionFlags {
        using T = uint8_t;
        T value;
        ebl::BitField<T, 0, 1> cap_read;
        ebl::BitField<T, 1, 1> cap_write;
        ebl::BitField<T, 2, 1> cap_execute;
        ebl::BitField<T, 3, 1> cap_specific;
        ebl::BitField<T, 0, 4> capability;
        ebl::BitField<VmRegionType, 4, 2> type;
    };

    struct VmRegion final : ebl::RefCountable<VmRegion*>, ebl::IntrusiveListNode<VmRegion> {
    public:
        status_t allocate_vmr_compact(
            size_t size, uint8_t align_pow2, VmRegionFlags flags,
            ebl::RefPtr<VmRegion>& vmr_out);
        status_t allocate_vmr_sparse(
            size_t size, uint8_t align_pow2, VmRegionFlags flags,
            ebl::RefPtr<VmRegion>& vmr_out);
        status_t map_pages(
            vaddr_t offset, size_t size, VmRegionFlags flags,
            ebl::RefPtr<VmObject> object, vaddr_t vmo_offset,
            arch::mmu_flags mmu_flags, ebl::RefPtr<VmRegion>& map_out);
        status_t protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags);
    private:
        status_t split(vaddr_t offset, size_t size, ebl::RefPtr<VmRegion>& vmr_out);
    private:
        vaddr_t base_;
        vaddr_t size_;
        VmRegionFlags flags_;
        ebl::RefPtr<VmRegion> parent_;
        ebl::RefPtr<AddressSpace> aspace_;
        ebl::RefPtr<VmObject> object_;
        ebl::IntrusiveList<VmRegion> children_;
    };

    struct AddressSpace final : ebl::RefCountable<AddressSpace*> {
    public:
        arch::AddressSpace& arch() { return backend_; }
    private:
        VmRegion user_root_;
        VmRegion kernel_root_;
        arch::AddressSpace backend_;
        core::Spinlock lock_;
    };

    struct ABICOMPAT Page final : ebl::IntrusiveListNode<Page> {
        uint32_t flags;
        core::Spinlock lock;
        union {
            vaddr_t u3_;
            struct AddressSpace* address_space;
        };
        paddr_t paddr() const {
            return (this - g::loader_state_.pfndb_arr) * arch::page_size;
        }
    };
}
