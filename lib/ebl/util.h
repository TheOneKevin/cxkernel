#pragma once

#include <ebl/type_traits.h>

#define ABI_ALIGN 8 // FIXME: Modify per-architecture
#define FENCE do { __asm__ volatile("" ::: "memory"); } while(0)
#define STRINGIFY_HIDDEN(x) #x
#define STRINGIFY(x) STRINGIFY_HIDDEN(x)
#define PACKED __attribute__((packed))
#define ABICOMPAT __attribute__ ((aligned(ABI_ALIGN)))

#define DELETE_COPY_ASSIGN(T) \
    T(const T&) = delete; T& operator=(const T&) = delete;

#define DELETE_MOVE_ASSIGN(T) \
    T(T&&) = delete; T& operator=(T&&) = delete

// std::forward<T> and std::move implementations
// ref: https://stackoverflow.com/questions/7510182/how-does-stdmove-transfer-values-into-rvalues
// ref: https://stackoverflow.com/questions/27501400/the-implementation-of-stdforward
namespace ebl {
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
    static inline T max(T const& a, T const& b) {
        return a < b ? b : a;
    }

    template<typename T>
    static inline T min(T const& a, T const& b) {
        return a < b ? a : b;
    }
} // namespace ebl
