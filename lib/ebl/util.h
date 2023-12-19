#pragma once

#include <ebl/type_traits.h>
#include <ebl/source_location.h>

#ifndef ABI_ALIGN
    #error "ABI_ALIGN not defined (check your build setup)"
#endif

#define FENCE do { __asm__ volatile("" ::: "memory"); } while(0)
#define STRINGIFY_HIDDEN(x) #x
#define STRINGIFY(x) STRINGIFY_HIDDEN(x)
#define PACKED __attribute__((packed))
#define ABICOMPAT __attribute__ ((aligned(ABI_ALIGN)))

#define DELETE_COPY_CTOR(T) T(const T&) = delete;
#define DELETE_COPY_ASSIGN(T) T& operator=(const T&) = delete;
#define DELETE_COPY(T) DELETE_COPY_CTOR(T) DELETE_COPY_ASSIGN(T)
#define DELETE_MOVE_CTOR(T) T(T&&) = delete;
#define DELETE_MOVE_ASSIGN(T) T& operator=(T&&) = delete;
#define DELETE_MOVE(T) DELETE_MOVE_CTOR(T) DELETE_MOVE_ASSIGN(T)

// std::forward<T> and std::move implementations
// ref: https://stackoverflow.com/questions/7510182/how-does-stdmove-transfer-values-into-rvalues
// ref: https://stackoverflow.com/questions/27501400/the-implementation-of-stdforward
namespace ebl {
    using nullptr_t = decltype(nullptr);

    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr typename remove_reference<T>::type&& move(T&& arg)
    {
        return static_cast<typename remove_reference<T>::type&&>(arg);
    }

    template<typename T>
    constexpr T max(T const& a, T const& b) {
        return a < b ? b : a;
    }

    template<typename T>
    constexpr T min(T const& a, T const& b) {
        return a < b ? a : b;
    }

    template<typename T>
    constexpr T align_down(T const& val, T const& align) {
        return val & ~(align - 1);
    }

    template<typename T>
    constexpr T align_up(T const& val, T const& align) {
        return (val + align - 1) & ~(align - 1);
    }
} // namespace ebl
