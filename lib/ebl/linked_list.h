#pragma once

#include <ebl/type_traits.h>
#include <stddef.h>

#include "arch/types.h"
#include "memory.h"

namespace ebl {

   template <typename T, int N>
   class ABICOMPAT PACKED IntrusiveListNode;

   template <typename T, int i>
   class ABICOMPAT PACKED IntrusiveListInternal;

   //===-------------------------------------------------------------------===//
   // has_member_numberoflists_

   namespace {
      template <class, class = void>
      struct has_member_numberoflists_ : ebl::false_type {};

      template <class T>
      struct has_member_numberoflists_<T, ebl::void_t<decltype(&T::NumberOfLists)>>
            : ebl::true_type {};
   }; // namespace

   //===-------------------------------------------------------------------===//
   // IntrusiveListNode

   /**
    * @brief A node for an intrusive list. Classes that want to be used in an
    *        intrusive list must inherit from this class.
    *
    * @tparam T The type of the node. If T is a RefPtr, then T is the type of the
    *           RefPtr, otherwise T is the type of the pointer.
    * @tparam N The number of lists this node is in. Defaults to 1.
    */
   template <typename T, int N = 1>
   class ABICOMPAT PACKED IntrusiveListNode {
   public:
      constexpr static int NumberOfLists = N;

   private:
      // Assert T is not RefPtr
      static_assert(!is_instance_v<T, RefPtr>, "IntrusiveListNode cannot be used with RefPtr<T>.");
      template <typename V, int i>
      friend class IntrusiveListInternal;

   public:
      IntrusiveListNode() : next{nullptr}, prev{nullptr} {};
      ~IntrusiveListNode() {
         for(int i = 0; i < N; ++i) {
            next[i] = nullptr;
            prev[i] = nullptr;
         }
      }

   private:
      template <int i = 0>
      void insert_after(T* node) {
         static_assert(i < N);
         if(node == nullptr || node == this) return;
         node->next[i] = next[i];
         node->prev[i] = this;
         next[i]->prev[i] = node;
         next[i] = node;
      }
      template <int i = 0>
      void insert_before(T* node) {
         static_assert(i < N);
         if(node == nullptr || node == this) return;
         node->prev[i] = prev[i];
         node->next[i] = this;
         prev[i]->next[i] = node;
         prev[i] = node;
      }

   public:
#ifdef LOADER
      // Shift pointers by an offset
      void shift(vaddr_t offset) {
         for(int i = 0; i < N; ++i) {
            if(virt0_[i] != 0) virt0_[i] += offset;
            if(virt1_[i] != 0) virt1_[i] += offset;
         }
      }
#endif

   private:
      union {
         T* next[N];
         vaddr_t virt0_[N];
      };
      union {
         T* prev[N];
         vaddr_t virt1_[N];
      };
   };

   //===-------------------------------------------------------------------===//
   // IntrusiveListInternal

   template <typename T, int i = 0>
   class ABICOMPAT PACKED IntrusiveListInternal {
      union {
         T* root;
         vaddr_t virt0_;
      };

   protected:
      static consteval void check_validity() {
         static_assert(has_member_numberoflists_<T>::value,
                       "IntrusiveListInternal must be used with a type T such that T has a static "
                       "member NumberOfLists");
         static_assert(is_base_of_v<IntrusiveListNode<T, T::NumberOfLists>, T>,
                       "IntrusiveListInternal must be used with a type T such that T inherits from "
                       "IntrusiveListNode");
         static_assert(!is_instance_v<T, RefPtr>,
                       "IntrusiveListInternal cannot be used with RefPtr<T>.");
         static_assert(i < T::NumberOfLists,
                       "IntrusiveListInternal must be used with a type T such that i < "
                       "T::NumberOfLists");
      }

   public:
      /// Iterator for the list
      struct iterator;

   public:
#ifdef LOADER
      // Shift pointers by an offset
      void shift(vaddr_t offset) {
         check_validity();
         if(virt0_ != 0) virt0_ += offset;
      }
#endif
      // Returns if this list is empty.
      bool empty() const {
         check_validity();
         return root == nullptr;
      }
      // Returns an iterator to the beginning of the list.
      iterator begin() const {
         check_validity();
         return iterator(root, false);
      }
      // Returns an iterator to the end of the list.
      iterator end() const {
         check_validity();
         return iterator(root, root != nullptr);
      }
      // Get list size
      size_t size() const {
         check_validity();
         size_t size = 0;
         for(auto it = begin(); it != end(); ++it, ++size)
            ;
         return size;
      }

   protected:
      // Constructs an empty list.
      IntrusiveListInternal() : root{nullptr} {};
      // Returns a reference to the data contained in the first node.
      T* first_unsafe() const {
         check_validity();
         return root;
      }
      // Returns a reference to the data contained in the last node.
      T* last_unsafe() const {
         check_validity();
         if(root == nullptr) return nullptr;
         return root->prev[i];
      }
      // Inserts new_node before node. Does nothing if the
      // node is nullptr or equal to new_node.
      void insert_before_unsafe(T* node, T* new_node) {
         check_validity();
         if(node == nullptr || new_node == nullptr || node == new_node) return;
         node->prev[i]->next[i] = new_node;
         new_node->prev[i] = node->prev[i];
         new_node->next[i] = node;
         node->prev[i] = new_node;
      }
      // Remove node from the list. Does nothing if the node is nullptr.
      void remove_unsafe(T* node) {
         check_validity();
         if(node == nullptr) return;
         node->prev[i]->next[i] = node->next[i];
         node->next[i]->prev[i] = node->prev[i];
         node->prev[i] = nullptr;
         node->next[i] = nullptr;
      }
      // push_front but with raw pointers
      void push_front_unsafe(T* node) {
         check_validity();
         if(root == nullptr) {
            root = node;
            node->prev[i] = node;
            node->next[i] = node;
         } else {
            insert_before_unsafe(root, node);
            root = node;
         }
      }
      // push_back but with raw pointers
      void push_back_unsafe(T* node) {
         check_validity();
         if(root == nullptr) {
            root = node;
            node->prev[i] = node;
            node->next[i] = node;
         } else {
            insert_before_unsafe(root, node);
         }
      }
      // pop_front but with raw pointers
      T* pop_front_unsafe() {
         check_validity();
         T* node = root;
         if(root == nullptr) // Size of list is 0
            return nullptr;
         if(root->next[i] == root) { // Size of list is 1
            root = nullptr;
            return node;
         }
         root = root->next[i];
         remove_unsafe(node);
         return node;
      }
      // pop_back but with raw pointers
      T* pop_back_unsafe() {
         check_validity();
         T* node = root->prev[i];
         if(root == nullptr) // Size of list is 0
            return nullptr;
         if(root == node) { // Size of list is 1
            root = nullptr;
            return node;
         }
         remove_unsafe(node);
         if(node == root) root = nullptr;
         return node;
      }

   public:
      // An iterator for the list, implements required iterator functions.
      struct iterator {
         T* operator*() const { return list; }
         T* operator->() const { return list; }
         iterator& operator++() {
            flag = true;
            list = list->next[i];
            return *this;
         }
         bool operator==(const iterator& a) const { return list == a.list && flag == a.flag; };
         bool operator!=(const iterator& a) const { return list != a.list || flag != a.flag; };

      private:
         friend class IntrusiveListInternal;
         iterator(T* x, bool flag) : list{x}, flag{flag} {};

      private:
         T* list;
         bool flag;
      };
   };

   //===-------------------------------------------------------------------===//
   // IntrusiveList<T> where T inherits from IntrusiveListNode

   /**
    * @brief An intrusive list. T must inherit from IntrusiveListNode.
    *
    * @tparam T The type of the node. T must inherit from IntrusiveListNode.
    * @tparam i The index of the list. Must be less than T::NumberOfLists.
    */
   template <typename T, int i = 0>
   class ABICOMPAT PACKED IntrusiveList final : public IntrusiveListInternal<T, i> {
      using Base = IntrusiveListInternal<T, i>;

   public:
      IntrusiveList() : Base{} {};
      ~IntrusiveList() {}
      void push_front(T* node) { Base::push_front_unsafe(node); }
      void push_back(T* node) { Base::push_back_unsafe(node); }
      T* pop_front() { return Base::pop_front_unsafe(); }
      T* pop_back() { return Base::pop_back_unsafe(); }
      T* remove(T* node) {
         Base::remove_unsafe(node);
         return node;
      }
      void insert_before(T* inspt, T* node) { Base::insert_before_unsafe(inspt, node); }
   };

   //===-------------------------------------------------------------------===//
   // IntrusiveList<RefPtr<T>> specialization

   /**
    * @brief A specialization of IntrusiveList<T> for RefPtr<T> where T inherits from
    *        IntrusiveListNode. The list will automatically add and remove references to the
    *        objects in the list.
    *
    * @tparam T The type of the node. T must inherit from IntrusiveListNode.
    * @tparam i The index of the list. Must be less than T::NumberOfLists.
    */
   template <typename T, int i>
   class ABICOMPAT PACKED IntrusiveList<RefPtr<T>, i> final : public IntrusiveListInternal<T, i> {
      using Base = IntrusiveListInternal<T, i>;

   public:
      IntrusiveList() : Base{} {};
      ~IntrusiveList() {}
      void push_front(RefPtr<T> node) {
         node.get()->add_ref();
         Base::push_front_unsafe(node.get());
      }
      void push_back(RefPtr<T> node) {
         node.get()->add_ref();
         Base::push_back_unsafe(node.get());
      }
      RefPtr<T> pop_front() {
         T* ptr = Base::pop_front_unsafe();
         ptr->release();
         return RefPtr<T>{ptr};
      }
      RefPtr<T> pop_back() {
         T* ptr = Base::pop_back_unsafe();
         ptr->release();
         return RefPtr<T>{ptr};
      }
      RefPtr<T> remove(RefPtr<T> node) {
         Base::remove_unsafe(node.get());
         return ebl::move(node);
      }
      void insert_before(T* inspt, RefPtr<T> node) {
         node.get()->add_ref();
         Base::insert_before_unsafe(inspt, node.get());
      }
   };

} // namespace ebl
