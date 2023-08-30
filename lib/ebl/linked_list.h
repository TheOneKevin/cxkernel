#pragma once

#include "ebl/type_traits.h"
#include "memory.h"
#include <stddef.h>
#include "arch/types.h"

namespace ebl {

template<typename T, int N = 1>
struct ABICOMPAT IntrusiveListNode {
    static constexpr int NumberOfLists = N;
    union {
        T* next[N];
        vaddr_t virt0_[N];
    };
    union {
        T* prev[N];
        vaddr_t virt1_[N];
    };
};

template<typename T, int i = 0>
class ABICOMPAT IntrusiveList {
    struct iterator;
    union {
        T* root;
        vaddr_t virt0_;
    };
    static consteval bool check_validity() {
        return (i < T::NumberOfLists) &&
            ebl::is_base_of_v<IntrusiveListNode<T, T::NumberOfLists>, T>;
    }
public:
    // Constructs an empty list.
    IntrusiveList(): root{nullptr} { };
#ifdef LOADER
    // Shift pointers by an offset
    void shift(vaddr_t offset) {
        static_assert(check_validity());
        if(virt0_ != 0)
            virt0_ += offset;
    }
#endif
    // Returns a reference to the data contained in the first node.
    T* first() const {
        static_assert(check_validity());
        return root;
    }
    // Returns a reference to the data contained in the last node.
    T* last() const {
        static_assert(check_validity());
        if(root == nullptr)
            return nullptr;
        return root->prev[i];
    }
    // Returns if this list is empty.
    bool empty() const {
        static_assert(check_validity());
        return root == nullptr;
    }
    // Inserts new_node before node. Does nothing if the
    // node is nullptr or equal to new_node.
    void insert_before_unsafe(T* node, T* new_node) {
        static_assert(check_validity());
        if(node == nullptr || new_node == nullptr || node == new_node)
            return;
        node->prev[i]->next[i] = new_node;
        new_node->prev[i] = node->prev[i];
        new_node->next[i] = node;
        node->prev[i] = new_node;
    }
    // Remove node from the list. Does nothing if the node is nullptr.
    void remove_unsafe(T* node) {
        static_assert(check_validity());
        if(node == nullptr)
            return;
        node->prev[i]->next[i] = node->next[i];
        node->next[i]->prev[i] = node->prev[i];
        node->prev[i] = nullptr;
        node->next[i] = nullptr;
    }
    // push_front but with raw pointers
    void push_front_unsafe(T* node) {
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
    void push_back_unsafe(T* node) {
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
    T* pop_front_unsafe() {
        static_assert(check_validity());
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
        static_assert(check_validity());
        T* node = root->prev[i];
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
        for(auto it = begin(); it != end(); ++it, ++size);
        return size;
    }
private:
    // An iterator for the list, implements required iterator functions.
    struct iterator {
        iterator(T* x, bool flag): list{x}, flag{flag} { };
        T& operator *() const { return *list; }
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
        T* list;
        bool flag;
    };
};

} // namespace ebl
