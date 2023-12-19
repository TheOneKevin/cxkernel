#pragma once

#include "loaderabi.h"
#include "arch/types.h"
#include "core/spinlock.h"
#include "core/mutex.h"
#include <stdint.h>
#include <ebl/bit.h>
#include <ebl/status.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {
    struct ABICOMPAT Page;
    struct VmObject;
    struct VmRegion;
    struct AddressSpace;

    struct VmObject final : ebl::RefCountable<VmObject> {
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

    struct VmRegion final : ebl::RefCountable<VmRegion>,
                            ebl::IntrusiveListNode<ebl::RefPtr<VmRegion>> {
        friend struct AddressSpace;
    public:
        VmRegion() noexcept {};
        Result<ebl::RefPtr<VmRegion>>
        allocate_vmr_compact(size_t size, uint8_t align_pow2, VmRegionFlags flags);
        Result<ebl::RefPtr<VmRegion>>
        allocate_vmr_sparse(size_t size, uint8_t align_pow2, VmRegionFlags flags);
        Result<ebl::RefPtr<VmRegion>>
        map_pages(
            vaddr_t offset, size_t size, VmRegionFlags flags,
            ebl::RefPtr<VmObject> object, vaddr_t vmo_offset,
            arch::mmu_flags mmu_flags
        );
        Result<void> protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags);
    private:
        Result<ebl::RefPtr<VmRegion>> split(vaddr_t offset, size_t size);
    private:
        vaddr_t base_;
        vaddr_t size_;
        VmRegionFlags flags_;
        VmRegion* parent_;
        AddressSpace* aspace_;
        ebl::RefPtr<VmObject> object_;
        ebl::IntrusiveList<ebl::RefPtr<VmRegion>> children_;
    };

    struct AddressSpace final : ebl::RefCountable<AddressSpace> {
    public:
        static ebl::RefPtr<AddressSpace> Create() {
            return ebl::RefPtr<AddressSpace>(new AddressSpace());
        }
        arch::AddressSpace& arch() { return backend_; }
    public:
        AddressSpace() noexcept {
            
        }
    private:
        VmRegion user_root_;
        VmRegion kernel_root_;
        arch::AddressSpace backend_;
        mutable core::Mutex lock_;
    };

    struct ABICOMPAT PACKED Page final : ebl::IntrusiveListNode<Page> {
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
