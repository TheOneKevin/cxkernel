#pragma once

// A lot of the functions are re-implementations from libcxx
// ref: https://github.com/llvm-mirror/libcxx

namespace ebl {

namespace internal {
    template <class T, T v> struct integral_constant {
        static constexpr const T value = v;
    };
    template <bool B> using bool_constant = integral_constant<bool, B>;
    template <class T> T&& __declval(int) {}
    template <class T> T __declval(long) {}
} // namespace internal

typedef internal::bool_constant<true> true_type;
typedef internal::bool_constant<false> false_type;
template <typename...> using void_t = void;

template <class T, class U> struct is_same : public false_type {};
template <class T> struct is_same<T, T> : public true_type {};
template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template <class T>
decltype(internal::__declval<T>(0)) declval() noexcept {}

template <class, class T, class... Args>
struct is_constructible_ : false_type {};

template <class T, class... Args>
struct is_constructible_<
    void_t<decltype(T(declval<Args>()...))>, T, Args...> : true_type {};

template <class T, class... Args>
using is_constructible = is_constructible_<void_t<>, T, Args...>;

template <class T, class... Args>
inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

template <bool, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> {typedef T type;};

template <class T, class M> M get_member_type(M T:: *);

template <class... T> struct always_false : ebl::false_type {};

/*===----------------------------------------------------------------------===*/
// ref: ??? (I forgot where I got this from)

template<class T>
struct remove_reference { typedef T type; };
template<class T>
struct remove_reference<T&> { typedef T type; };
template<class T>
struct remove_reference<T&&> { typedef T type; };

/*===----------------------------------------------------------------------===*/
// ref: https://en.cppreference.com/w/cpp/experimental/is_detected

namespace internal {
    template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
    struct detector {
        using value_t = false_type;
        using type = Default;
    };
 
    template<class Default, template<class...> class Op, class... Args>
    struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
        using value_t = true_type;
        using type = Op<Args...>;
    };
} // namespace internal

struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

template<template<class...> class Op, class... Args>
using is_detected = typename internal::detector<nonesuch, void, Op, Args...>::value_t;
 
template<template<class...> class Op, class... Args>
using detected_t = typename internal::detector<nonesuch, void, Op, Args...>::type;
 
template<class Default, template<class...> class Op, class... Args>
using detected_or = internal::detector<Default, void, Op, Args...>;

template<class Expected, template<class...> class Op, class... Args>
using is_detected_exact = is_same<Expected, detected_t<Op, Args...>>;

template<class Expected, template<class...> class Op, class... Args>
inline constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

} // namespace ebl
