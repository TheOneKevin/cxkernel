#pragma once

namespace ebl {

/**
 * @brief A type trait to associate a linked list node with its value type.
 * 
 * @tparam T Type of the list node's value.
 */
template<typename T> struct LinkedRef { typedef T type; };

/**
 * @brief Macro to associate a linked list node with its value type.
 *        No other linked-list node types can use the same value type.
 *        This will ensure any LPtr<T> will be able to uniquely convert to T*.
 */
#define MakeLinkedRef(T) \
    template<> struct ebl::LinkedRef<T> { typedef T##_node type; }

/**
 * @brief Does the opposite of MakeLinkedRef. This is useful for lists whose
 *        nodes are NOT heap-allocated. So LPtr<T> is illegal to use (as it
 *        will attempt to delete the node).
 */
#define ProhibitLinkedRef(T) \
    template<> struct ebl::LinkedRef<T> { typedef void type; }

/**
 * @brief A unique pointer to a linked list node.
 * @ref ebl::IntrusiveListNode
 * 
 * @tparam T Type of the list node's value.
 */
template<typename T>
class LPtr {
    using U = typename LinkedRef<T>::type;
public:
    LPtr() noexcept : ptr_{nullptr} { }
    LPtr(LPtr&& other) noexcept : ptr_{other.ptr_} {
        other.ptr_ = nullptr;
    }
    LPtr(U* ref) noexcept : ptr_{ref} { }
    LPtr(const LPtr&) = delete;
    ~LPtr() noexcept {
        delete ptr_;
    }
    LPtr& operator=(LPtr&& other) noexcept {
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }
    LPtr& operator=(const LPtr&) = delete;
    T* operator->() const noexcept {
        return convert(ptr_);
    }
    T& operator*() const noexcept {
        return *convert(ptr_);
    }
    T* get() const noexcept {
        return convert(ptr_);
    }
    void release() noexcept {
        ptr_ = nullptr;
    }
private:
    constexpr T* convert(U* ptr) const noexcept {
        return ptr == nullptr ? nullptr : static_cast<T*>(*ptr);
    }
private:
    U* ptr_;
};

} // namespace ebl
