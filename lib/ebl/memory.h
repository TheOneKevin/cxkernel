#pragma once

#include <ebl/assert.h>
#include <ebl/status.h>
#include <stdint.h>

#include "ebl/atomic.h"
#include "ebl/new.h"
#include "ebl/util.h"

namespace kmem {
   void* alloc(unsigned int size);
}

namespace ebl {

   /**
    * @brief A reference-counted pointer to an object. Similar to std::shared_ptr.
    *
    * @tparam T Type of the object.
    */
   template <typename T>
   class RefPtr;

   template <typename T, int i>
   class IntrusiveList;

   /**
    * @brief Intrusively reference-counted object. Objects wishing to be ref-counted
    * (i.e., be used with RefPtr) should inherit from this class.
    *
    * @tparam T Unused type parameter to ensure base class uniqueness (i.e.,
    * unique ref-countable objects won't be upcasted to the same base class)
    */
   template <typename T>
   class RefCountable {
      template <typename V>
      friend class RefPtr;

      template <typename V, int i>
      friend class IntrusiveList;

   public:
      DELETE_COPY(RefCountable);
      DELETE_MOVE(RefCountable);
      RefCountable() noexcept : ref_count_(0) {}
      ~RefCountable() noexcept {}

   private:
      void add_ref() const noexcept { ref_count_.fetch_add(1, memory_order_relaxed); }
      bool release() const noexcept {
         auto ref = ref_count_.fetch_sub(1, memory_order_release);
         if(ref == 1) {
            atomic_thread_fence(memory_order_acquire);
            return true;
         }
         return false;
      }
      void adopt() const noexcept { ref_count_.store(1, memory_order_relaxed); }

   private:
      mutable atomic<int32_t> ref_count_;
   };

   /**
    * @brief Wraps an RefPtr around a newly-allocated RefCountable object.
    *
    * @tparam T Object type.
    * @param ptr Pointer to the object.
    * @return RefPtr<T> Reference-counted pointer to the object.
    */
   template <typename T>
   RefPtr<T> AdoptRef(T* ptr);

   template <typename T>
   class RefPtr final {
   public:
      constexpr RefPtr() noexcept : ptr_{nullptr} {}
      constexpr RefPtr(ebl::nullptr_t) noexcept : ptr_{nullptr} {}
      // Construct from raw pointer, pointer must already be adopted
      RefPtr(T* ptr) noexcept : ptr_{ptr} {
         if(ptr_ != nullptr) {
            ptr_->add_ref();
         }
      }
      // Copy constructor
      RefPtr(RefPtr const& other) : RefPtr{other.ptr_} {}
      // Move constructor
      RefPtr(RefPtr&& other) noexcept : ptr_{other.ptr_} { other.ptr_ = nullptr; }
      // Copy assignment
      RefPtr& operator=(RefPtr const& other) {
         if(other.ptr_ != nullptr) other.ptr_->add_ref();
         T* old = ptr_;
         ptr_ = other.ptr_;
         destroy(old);
         return *this;
      }
      // Move assignment (use copy-swap idiom)
      RefPtr& operator=(RefPtr&& other) noexcept {
         RefPtr{ebl::move(other)}.swap(*this);
         return *this;
      }
      // Destructor
      ~RefPtr() {
         destroy(ptr_);
         ptr_ = nullptr;
      }
      // Swap pointers (this does not affect reference count)
      void swap(RefPtr& other) noexcept {
         T* tmp = ptr_;
         ptr_ = other.ptr_;
         other.ptr_ = tmp;
      }
      T* get() const { return ptr_; }
      T& operator*() const { return *ptr_; }
      T* operator->() const { return ptr_; }
      explicit operator bool() const { return !!ptr_; }
      bool operator==(RefPtr const& other) const { return ptr_ == other.ptr_; }
      bool operator!=(RefPtr const& other) const { return ptr_ != other.ptr_; }
      bool operator==(ebl::nullptr_t) const { return ptr_ == nullptr; }
      bool operator!=(ebl::nullptr_t) const { return ptr_ != nullptr; }

   private:
      friend RefPtr<T> AdoptRef<T>(T*);
      enum AdoptTag { ADOPT };
      RefPtr(T* ptr, AdoptTag) : ptr_{ptr} {
         if(ptr_ != nullptr) {
            ptr_->adopt();
         }
      }
      static void destroy(T* ptr) {
         if(ptr != nullptr && ptr->release()) {
            delete ptr;
         }
      }

   private:
      T* ptr_;
   };

   template <typename T>
   inline RefPtr<T> AdoptRef(T* ptr) {
      return RefPtr<T>(ptr, RefPtr<T>::ADOPT);
   }

   template <typename T>
   struct MakeRefPtrHelper;

   template <typename T>
   struct MakeRefPtrHelper {
      template <typename... Args>
      static Result<RefPtr<T>> make_ref(Args&&... args) {
         T* ptr = (T*)kmem::alloc(sizeof(T));
         if(ptr == nullptr) return E::OUT_OF_MEMORY;
         ptr = new(ptr) T{ebl::forward<Args>(args)...};
         return AdoptRef(ptr);
      }
   };

   template <typename T, typename... Args>
   Result<RefPtr<T>> MakeRefPtr(Args&&... args) {
      return MakeRefPtrHelper<T>::make_ref(ebl::forward<Args>(args)...);
   }

} // namespace ebl
