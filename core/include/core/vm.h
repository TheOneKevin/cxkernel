#pragma once

#include <ebl/bit.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/status.h>
#include <stdint.h>

#include "arch/types.h"
#include "core/mutex.h"
#include "core/spinlock.h"
#include "core/vmregion.h"
#include "loaderabi.h"

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

   struct AddressSpace final : ebl::RefCountable<AddressSpace> {
      friend void arch::init_aspace(AddressSpace&);

     public:
      arch::AddressSpace& arch() { return backend_; }

     public:
      AddressSpace() noexcept : user_root_{0, 0, {}, this}, kernel_root_{0, 0, {}, this} {
         user_root_.flags_.capability = 0b1111;
         kernel_root_.flags_.capability = 0b1111;
         user_root_.flags_.type = VmRegionType::REGION;
         kernel_root_.flags_.type = VmRegionType::REGION;
         user_root_.flags_.is_root = 1;
         kernel_root_.flags_.is_root = 1;
      }

      VmRegion& get_user_root() { return user_root_; }

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
      paddr_t paddr() const { return (this - g::loader_state_.pfndb_arr) * arch::page_size; }
   };
} // namespace core
