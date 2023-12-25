#pragma once

#include <arch/interface.h>
#include <ebl/bit.h>
#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/status.h>
#include <stdint.h>

namespace core {
   struct VmObject;
   struct AddressSpace;
   struct VmRegion;

   enum struct VmRegionType : uint8_t { REGION = 0, MAPPING = 1 };

   union VmRegionFlags {
      using T = uint8_t;
      T value;
      ebl::BitField<T, 0, 1> cap_read;
      ebl::BitField<T, 1, 1> cap_write;
      ebl::BitField<T, 2, 1> cap_execute;
      ebl::BitField<T, 3, 1> cap_specific;
      ebl::BitField<T, 0, 4> capability;
      ebl::BitField<VmRegionType, 4, 1> type;
      ebl::BitField<T, 5, 1> is_root;
   };

   struct VmRegion final : ebl::RefCountable<VmRegion>,
                           ebl::IntrusiveListNode<VmRegion> {
      friend struct AddressSpace;
      friend struct ebl::MakeRefPtrHelper<VmRegion>;
      friend void arch::init_aspace(AddressSpace&);

     private:
      VmRegion(vaddr_t base, vaddr_t size, VmRegionFlags flags, AddressSpace* aspace) noexcept
            : base_{base}, size_{size}, flags_{flags}, parent_{nullptr}, aspace_{aspace} {}

     public:
      /**
       * @brief
       *
       * @param size
       * @param align_pow2
       * @param flags
       * @return Result<ebl::RefPtr<VmRegion>>
       */
      Result<ebl::RefPtr<VmRegion>> allocate_vmr_compact(size_t size,
                                                         uint8_t align_pow2,
                                                         VmRegionFlags flags);

      /**
       * @brief
       *
       * @param size
       * @param align_pow2
       * @param flags
       * @return Result<ebl::RefPtr<VmRegion>>
       */
      Result<ebl::RefPtr<VmRegion>> allocate_vmr_sparse(size_t size,
                                                        uint8_t align_pow2,
                                                        VmRegionFlags flags);

      /**
       * @brief
       *
       * @param offset
       * @param size
       * @param flags
       * @param object
       * @param vmo_offset
       * @param mmu_flags
       * @return Result<ebl::RefPtr<VmRegion>>
       */
      Result<ebl::RefPtr<VmRegion>> map_pages(vaddr_t offset,
                                              size_t size,
                                              VmRegionFlags flags,
                                              ebl::RefPtr<VmObject> object,
                                              vaddr_t vmo_offset,
                                              arch::mmu_flags mmu_flags);

      /**
       * @brief
       *
       * @param addr
       * @param size
       * @param flags
       * @return Result<void>
       */
      Result<void> protect(vaddr_t addr, vaddr_t size, arch::mmu_flags flags);

      /**
       * @brief
       *
       * @return Result<void>
       */
      Result<void> destroy();

      auto begin() { return children_.begin(); }
      auto end() { return children_.end(); }

      auto get_base() const { return base_; }
      auto get_size() const { return size_; }

     private:
      /**
       * @brief Find the child region that is immediately above the given offset.
       *
       * @param offset Offset within this region.
       * @return ebl::RefPtr<VmRegion> A reference to the child region, or nullptr if none.
       */
      ebl::RefPtr<VmRegion> find_child_above(vaddr_t offset);

      /**
       * @brief Check if the given offset and size overlaps with any child region.
       *
       * @param offset Offset within this region.
       * @param size Size of the region to check.
       */
      bool does_overlap_child(vaddr_t offset, size_t size);

      /**
       * @brief Iterate over all holes in this region.
       *
       * @tparam T   Type bool(vaddr_t, size_t) that returns true to continue, false to stop.
       * @param func The function to call for each hole, with type T.
       */
      template <typename T>
      void foreach_hole(T func) {
         // TODO: Type check T using this (need to implement these templates first)
         // static_assert(is_convertible_v<T, function<bool(vaddr_t, size_t)>>, "Error...");
         vaddr_t offset = 0;
         for(auto child : children_) {
            if(child->base_ > this->base_ + offset) {
               if(!func(child, offset, child->base_ - this->base_ - offset)) return;
            }
            offset = child->base_ + child->size_;
         }
         if(this->base_ + this->size_ > this->base_ + offset) {
            func(nullptr, offset, this->base_ + this->size_ - this->base_ - offset);
         }
      }

     private:
      /// The (absolute) base address of this region.
      vaddr_t base_;
      /// The size of this region.
      vaddr_t size_;
      /// Region capabilities and type.
      VmRegionFlags flags_;
      /// The parent region of this region, or nullptr if this is the root.
      VmRegion* parent_;
      /// The address space this region belongs to.
      AddressSpace* aspace_;
      /// The object backing this region, or nullptr if this is a hole.
      ebl::RefPtr<VmObject> object_;
      /// Ordered list of child regions, sorted by base address.
      ebl::IntrusiveList<ebl::RefPtr<VmRegion>> children_;
   };
} // namespace core
