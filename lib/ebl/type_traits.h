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

// conditional
template<bool B, class T, class F> struct conditional { typedef T type; };
template<class T, class F> struct conditional<false, T, F> { typedef F type; };
template<bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

// is_base_of
template <class B, class D>
struct is_base_of : public internal::integral_constant<bool, __is_base_of(B, D)> {};
template <class B, class D>
inline constexpr bool is_base_of_v = is_base_of<B, D>::value;

// is_instance
template <class, template <class...> class>
struct is_instance : public false_type {};
template <class T, template <class...> class U>
struct is_instance<U<T>, U> : public true_type {};
template <class T, template <class...> class U>
inline constexpr bool is_instance_v = is_instance<T, U>::value;

// is_class
template <class T> struct is_class
    : public internal::integral_constant<bool, __is_class(T)> {};
template <class T> inline constexpr bool is_class_v = is_class<T>::value;

// has_virtual_destructor
template <class T> struct has_virtual_destructor
    : public internal::integral_constant<bool, __has_virtual_destructor(T)> {};
template <class T>
inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

// is_integral
template <class T>  struct is_integral                     : public false_type {};
template <>         struct is_integral<bool>               : public true_type {};
template <>         struct is_integral<char>               : public true_type {};
template <>         struct is_integral<signed char>        : public true_type {};
template <>         struct is_integral<unsigned char>      : public true_type {};
template <>         struct is_integral<wchar_t>            : public true_type {};
template <>         struct is_integral<short>              : public true_type {};
template <>         struct is_integral<unsigned short>     : public true_type {};
template <>         struct is_integral<int>                : public true_type {};
template <>         struct is_integral<unsigned int>       : public true_type {};
template <>         struct is_integral<long>               : public true_type {};
template <>         struct is_integral<unsigned long>      : public true_type {};
template <>         struct is_integral<long long>          : public true_type {};
template <>         struct is_integral<unsigned long long> : public true_type {};
template <class T>  inline constexpr bool is_integral_v = is_integral<T>::value;

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

/*===----------------------------------------------------------------------===*/
// Determines if T is a complete type. Requires clang-13 or later.
// ref: https://devblogs.microsoft.com/oldnewthing/20190710-00/?p=102678

template<typename T, typename G = decltype([](){})>
consteval bool is_type_complete() {
    return requires { typename void_t<decltype(sizeof(T))>; };
}

/*===----------------------------------------------------------------------===*/
// is_trivially_destructible

template <class T> struct is_trivially_destructible:
    public internal::integral_constant<bool, __is_trivially_destructible(T)>{};

} // namespace ebl
