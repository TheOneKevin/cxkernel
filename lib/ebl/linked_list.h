#pragma once

#include "memory.h"
#include <stddef.h>
#include "arch/types.h"

namespace ebl {

//! @brief Checks if the LinkedRef type matches the IntrusiveListNode type.
//!        This is used to ensure that the LPtr<T> type is used correctly.
//!        See: MakeLinkedRef, ProhibitLinkedRef
#define LPtrCheck \
    static_assert( \
        ebl::is_same_v<IntrusiveListNode<T, N>, typename LinkedRef<T>::type>, \
        "LinkedRef<T>::type does not match the IntrusiveListNode<T, N> caller");

// A node in the list, containing both data and intrusive pointers.
template<typename T, int N>
class ABICOMPAT IntrusiveListNode {
    using list_node = IntrusiveListNode;
public:
    union {
        list_node* prev[N];
        vaddr_t virt0_[N];
    };
    union {
        list_node* next[N];
        vaddr_t virt1_[N];
    };
    T value;
    explicit operator T*() { return &value; }

private:
    // An iterator for the list, implements required iterator functions.
    template<int i>
    class iterator {
    public:
        iterator(list_node* x, bool flag): list{x}, flag{flag} { };
        T& operator *() const { return list -> value; }
        iterator& operator ++() {
            flag = true;
            list = list -> next[i];
            return *this;
        }
        bool operator== (const iterator& a) const {
            return list == a.list && flag == a.flag; };
        bool operator!= (const iterator& a) const {
            return list != a.list || flag != a.flag; };
    private:
        list_node* list;
        bool flag;
    };

public:
    static list_node* container_of(T* ptr) {
        return (list_node*)((char*)ptr - offsetof(list_node, value));
    }

    template <typename... Args>
    static LPtr<T> construct(Args&&... args) {
        auto node = new list_node(forward<Args>(args)...);
        return LPtr<T>{node};
    }

private:
    template<typename... Args>
    IntrusiveListNode(Args&&... args) noexcept
        : value{forward<Args>(args)...}, prev{nullptr}, next{nullptr} { };

public:
    // Selects a single list from the multilist
    // Implementation is based off a circularly linked list
    template<int i>
    struct ABICOMPAT list final {
    private:
        static_assert(i < N, "Multilist select index (i) cannot exceed total list count N");
        friend class IntrusiveMultilist;
        union {
            list_node* root;
            vaddr_t virt0_;
        };
    public:
        // Constructs an empty list.
        list(): root{nullptr} { };
#ifdef LOADER
        // Shift pointers by an offset
        void shift(vaddr_t offset) {
            if(virt0_ != 0)
                virt0_ += offset;
        }
#endif
        // Returns a reference to the data contained in the first node.
        list_node* first() const {
            return root;
        }
        // Returns a reference to the data contained in the last node.
        list_node* last() const {
            if(root == nullptr)
                return nullptr;
            return root->prev[i];
        }
        // Returns if this list is empty.
        bool empty() const {
            return root == nullptr;
        }
        // Inserts new_node before node. Does nothing if the
        // node is nullptr or equal to new_node.
        void insert_before_unsafe(list_node* node, list_node* new_node) {
            if(node == nullptr || new_node == nullptr || node == new_node)
                return;
            node->prev[i]->next[i] = new_node;
            new_node->prev[i] = node->prev[i];
            new_node->next[i] = node;
            node->prev[i] = new_node;
        }
        // Remove node from the list. Does nothing if the node is nullptr.
        void remove_unsafe(list_node* node) {
            if(node == nullptr)
                return;
            node->prev[i]->next[i] = node->next[i];
            node->next[i]->prev[i] = node->prev[i];
            node->prev[i] = nullptr;
            node->next[i] = nullptr;
        }
        // push_front but with raw pointers
        void push_front_unsafe(list_node* node) {
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
        void push_back_unsafe(list_node* node) {
            if(root == nullptr) {
                root = node;
                node->prev[i] = node;
                node->next[i] = node;
            } else {
                insert_before_unsafe(root, node);
            }
        }
        // pop_front but with raw pointers
        list_node* pop_front_unsafe() {
            list_node* node = root;
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
        list_node* pop_back_unsafe() {
            list_node* node = root->prev[i];
            if(root == nullptr) // Size of list is 0
                return nullptr;
            if(root == node) { // Size of list is 1
                root = nullptr;
                return node;
            }
            remove_unsafe(node);
            if(node == root)
                root = nullptr;
            return node;
        }
        // Pushes existing node to the back of the list.
        void push_back(LPtr<T> elem) {
            LPtrCheck;
            list_node* node = container_of(elem.get());
            push_back_unsafe(node);
            elem.release();
        }
        // Pushes existing node to the front of the list.
        void push_front(LPtr<T> elem) {
            LPtrCheck;
            list_node* node = container_of(elem.get());
            push_front_unsafe(node);
            elem.release();
        }
        // Pop the first node from the list.
        LPtr<T> pop_front() {
            LPtrCheck;
            return LPtr<T>{pop_front_unsafe()};
        }
        // Pop the last node from the list.
        LPtr<T> pop_back() {
            LPtrCheck;
            return LPtr<T>{pop_back_unsafe()};
        }
        // Returns an iterator to the beginning of the list.
        iterator<i> begin() const {
            return iterator<i>(root, false);
        }
        // Returns an iterator to the end of the list.
        iterator<i> end() const {
            return iterator<i>(root, root != nullptr);
        }
        // Get list size
        size_t size() const {
            size_t size = 0;
            for(auto it = begin(); it != end(); ++it, ++size);
            return size;
        }
    };
};

#undef LPtrCheck

} // namespace ebl
