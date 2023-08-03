#pragma once

namespace ebl {

/**
 * @brief A type trait to associate a linked list node with its value type.
 * 
 * @tparam T Type of the list node's value.
 */
template<typename T> class LinkedRef { typedef T type; };

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
