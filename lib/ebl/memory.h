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
 * @ref ebl::IntrusiveMultilist
 * 
 * @tparam T Type of the list node's value.
 */
template<typename T>
class LPtr {
    using V = typename LinkedRef<T>::type;
    using U = typename V::list_node;
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

/*template<typename T, typename... Args>
Ptr<T> make_unique(Args&&... args) noexcept {
    return Ptr<T>{new T(forward<Args>(args)...)};
}*/

} // namespace ebl
