#pragma once

#include <ebl/type_traits.h>
#include <stddef.h>

#include "arch/types.h"
#include "memory.h"

namespace ebl {

   template <typename T, int N>
   class ABICOMPAT PACKED IntrusiveListNode;

   template <typename T, int i>
   class ABICOMPAT PACKED IntrusiveList;

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
      template <typename V, int i>
      friend class IntrusiveList;
      // U = T if T is a RefPtr, otherwise U = T*
      using U = conditional_t<is_instance_v<T, RefPtr>, T, T*>;
      // Extract RefPtr type from T
      template <typename V>
      struct RefPtrType {
         using type = V;
      };
      template <typename V>
      struct RefPtrType<RefPtr<V>> {
         using type = V;
      };
      // If T is RefPtr, then W satisfies RefPtr<W> otherwise W = T
      using W = typename RefPtrType<T>::type;

     public:
      IntrusiveListNode() : next{nullptr}, prev{nullptr} {};
      ~IntrusiveListNode() {
         for(int i = 0; i < N; ++i) {
            next[i] = nullptr;
            prev[i] = nullptr;
         }
      }
      template <int i = 0>
      void insert_after(U node) {
         static_assert(i < N);
         if(node == nullptr || node.get() == this) return;
         node->next[i] = next[i];
         node->prev[i] = static_cast<W*>(this);
         next[i]->prev[i] = node;
         next[i] = node;
      }
      template <int i = 0>
      void insert_before(U node) {
         static_assert(i < N);
         if(node == nullptr || node.get() == this) return;
         node->prev[i] = prev[i];
         node->next[i] = static_cast<W*>(this);
         prev[i]->next[i] = node;
         prev[i] = node;
      }
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
      static constexpr int NumberOfLists = N;
      union {
         U next[N];
         vaddr_t virt0_[N];
      };
      union {
         U prev[N];
         vaddr_t virt1_[N];
      };
   };

   /**
    * @brief An intrusive list. Classes that want to be used in an intrusive list
    *        must inherit from IntrusiveListNode (and T given here must be the same
    *        type as in IntrusiveListNode).
    *
    * @tparam T The type of the node. If T is a RefPtr, then T is the type of the
    *           RefPtr, otherwise T is the type of the pointer.
    * @tparam i The index of the list to use. Must not be greater than the number
    *           of lists the node is in (N). Defaults to 0.
    */
   template <typename T, int i = 0>
   class ABICOMPAT PACKED IntrusiveList final {
      using U = conditional_t<is_instance_v<T, RefPtr>, T, T*>;
      union {
         U root;
         vaddr_t virt0_;
      };
      static consteval bool check_validity() {
         // FIXME: This is broken right now
         /*return (i < T::NumberOfLists) &&
             ebl::is_base_of_v<IntrusiveListNode<T, T::NumberOfLists>, T>;*/
         return true;
      }

     public:
      /// Iterator for the list
      struct iterator;

     public:
      // Constructs an empty list.
      IntrusiveList() : root{nullptr} {};
      ~IntrusiveList() { root.~U(); };
#ifdef LOADER
      // Shift pointers by an offset
      void shift(vaddr_t offset) {
         static_assert(check_validity());
         if(virt0_ != 0) virt0_ += offset;
      }
#endif
      // Returns a reference to the data contained in the first node.
      U first() const {
         static_assert(check_validity());
         return root;
      }
      // Returns a reference to the data contained in the last node.
      U last() const {
         static_assert(check_validity());
         if(root == nullptr) return nullptr;
         return root->prev[i];
      }
      // Returns if this list is empty.
      bool empty() const {
         static_assert(check_validity());
         return root == nullptr;
      }
      // Inserts new_node before node. Does nothing if the
      // node is nullptr or equal to new_node.
      void insert_before_unsafe(U node, U new_node) {
         static_assert(check_validity());
         if(node == nullptr || new_node == nullptr || node == new_node) return;
         node->prev[i]->next[i] = new_node;
         new_node->prev[i] = node->prev[i];
         new_node->next[i] = node;
         node->prev[i] = new_node;
      }
      // Remove node from the list. Does nothing if the node is nullptr.
      void remove_unsafe(U node) {
         static_assert(check_validity());
         if(node == nullptr) return;
         node->prev[i]->next[i] = node->next[i];
         node->next[i]->prev[i] = node->prev[i];
         node->prev[i] = nullptr;
         node->next[i] = nullptr;
      }
      // push_front but with raw pointers
      void push_front_unsafe(U node) {
         static_assert(check_validity());
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
      void push_back_unsafe(U node) {
         static_assert(check_validity());
         if(root == nullptr) {
            root = node;
            node->prev[i] = node;
            node->next[i] = node;
         } else {
            insert_before_unsafe(root, node);
         }
      }
      // pop_front but with raw pointers
      U pop_front_unsafe() {
         static_assert(check_validity());
         U node = root;
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
      U pop_back_unsafe() {
         static_assert(check_validity());
         U node = root->prev[i];
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
      // Returns an iterator to the beginning of the list.
      iterator begin() const {
         static_assert(check_validity());
         return iterator(root, false);
      }
      // Returns an iterator to the end of the list.
      iterator end() const {
         static_assert(check_validity());
         return iterator(root, root != nullptr);
      }
      // Get list size
      size_t size() const {
         static_assert(check_validity());
         size_t size = 0;
         for(auto it = begin(); it != end(); ++it, ++size)
            ;
         return size;
      }

     public:
      // An iterator for the list, implements required iterator functions.
      struct iterator {
         U operator*() const { return list; }
         U operator->() const { return list; }
         iterator& operator++() {
            flag = true;
            list = list->next[i];
            return *this;
         }
         bool operator==(const iterator& a) const { return list == a.list && flag == a.flag; };
         bool operator!=(const iterator& a) const { return list != a.list || flag != a.flag; };

        private:
         friend class IntrusiveList;
         iterator(U x, bool flag) : list{x}, flag{flag} {};

        private:
         U list;
         bool flag;
      };
   };

} // namespace ebl
