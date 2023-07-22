#pragma once

#include "memory.h"

namespace ebl {

/**
 * @brief
 * An intrusive linked-list data structure with multiple lists.
 * Objects can also be safely moved outside a list, see @ref ebl::LPtr.
 * @details
 * Here, we create a multi-list of integers, with two sub-lists.
 * Note the size of the intrusive multilist structure is 0 bytes.
 * Example USAGE:
 * @verbatim
    ebl::IntrusiveMultilist<2, int> mlist{};
    decltype(mlist)::list<0> list1{};
    decltype(mlist)::list<1> list2{};
    int main() {
        list1.push_back(mlist.construct(1));
        list1.push_back(mlist.construct(2));
        list2.push_back(list1.first());
        list2.push_back(mlist.construct(3));
        for (auto x : list1) printf("%d\n", x);
        for (auto x : list2) printf("%d\n", x);
    }
   @endverbatim
 * Example OUTPUT:
 * @verbatim
    1
    2
    1
    3
   @endverbatim
 * 
 * @tparam N Number of lists T will simultaneously be a part of.
 * @tparam T The type of the data contained in the list.
 */
template<int N, typename T>
class IntrusiveMultilist final {
    static_assert(N > 0, "Number of lists (N) must be greater than 0");
public:
    // A node in the list, containing both data and intrusive pointers.
    struct list_node {
        friend class IntrusiveMultilist;
        list_node* prev[N];
        list_node* next[N];
        T value;
        explicit operator T*() { return &value; }
    private:
        template<typename... Args>
        list_node(Args&&... args) noexcept
            : value{forward<Args>(args)...}, prev{nullptr}, next{nullptr} { };
    };

private:
    static list_node* container_of(T* ptr) {
        return (list_node*)((char*)ptr - offsetof(list_node, value));
    }

private:
    // An iterator for the list, implements required iterator functions.
    class iterator {
    public:
        iterator(int i, list_node* x): i{i}, list{x} { };
        T& operator *() const { return list -> value; }
        iterator& operator ++() { list = list -> next[i]; return *this; }
        bool operator== (const iterator& a) const { return list == a.list; };
        bool operator!= (const iterator& a) const { return list != a.list; };
    private:
        list_node* list;
        int i;
    };

public:
    // Selects a single list from the multilist
    template<int i>
    class list final {
        static_assert(i < N, "Multilist select index (i) cannot exceed total list count N");
        friend class IntrusiveMultilist;
        list_node* root;
        list_node* tail; // FIXME: Is this needed?
    public:
        // Constructs an empty list.
        list(): root{nullptr}, tail{nullptr} { };
        // Returns a reference to the data contained in the first node.
        list_node* first() const {
            return root;
        }
        // Returns a reference to the data contained in the last node.
        list_node* last() const {
            return tail;
        }
        // Returns if this list is empty.
        bool empty() const {
            return root == nullptr;
        }
        // Pushes existing node to the front of the list.
        void push_front(LPtr<T> elem) {
            list_node* node = container_of(elem.get());
            if(root != nullptr) {
                node->next[i] = root;
                root->prev[i] = node;
            } else {
                tail = node;
            }
            root = node;
            elem.release();
        }
        // Pushes existing node to the back of the list.
        void push_back(LPtr<T> elem) {
            list_node* node = container_of(elem.get());
            if(root == nullptr) {
                root = node;
            } else {
                node->prev[i] = tail;
                tail->next[i] = node;
            }
            tail = node;
            elem.release();
        }
        // Pop the first node from the list.
        LPtr<T> pop_front() {
            if(root == nullptr) return nullptr;
            list_node* node = root;
            root = root->next[i];
            if(root != nullptr) {
                root->prev[i] = nullptr;
            } else {
                tail = nullptr;
            }
            return LPtr<T>{node};
        }
        // Pop the last node from the list.
        LPtr<T> pop_back() {
            if(root == nullptr) return nullptr;
            list_node* node = tail;
            tail = tail->prev[i];
            if(tail != nullptr) {
                tail->next[i] = nullptr;
            } else {
                root = nullptr;
            }
            return LPtr<T>{node};
        }
        // Returns an iterator to the beginning of the list.
        iterator begin() const { return iterator(i, root); }
        // Returns an iterator to the end of the list.
        iterator end() const { return iterator(i, nullptr); }
    };

public:
    // Constructs an empty list.
    IntrusiveMultilist() noexcept { };
    // Construct a new node and do nothing with it
    template <typename... Args>
    LPtr<T> construct(Args&&... args) {
        auto node = new list_node(forward<Args>(args)...);
        return LPtr<T>{node};
    }
};

} // namespace ebl
