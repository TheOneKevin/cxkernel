#pragma once

#include "ebl/type_traits.h"

namespace ebl {

template<class T>
class Guard final {
    using Policy = typename T::Policy;
    template<class V> using has_lock = decltype(declval<V&>().lock());
    template<class V> using has_unlock = decltype(declval<V&>().unlock());
    static_assert(ebl::is_detected_exact_v<void, has_lock, Policy>,
        "T::Policy must support public member: void lock(void)");
    static_assert(ebl::is_detected_exact_v<void, has_unlock, Policy>,
        "T::Policy must support public member: void unlock(void)");
    static_assert(ebl::is_constructible_v<Policy, T*>,
        "T::Policy must be constructible with T*");

public:
    explicit Guard(T* lock) : impl_{Policy{lock}} {
        impl_.lock();
    }
    void release() {
        impl_.unlock();
    }
    ~Guard() {
        release();
    }

private:
    Policy impl_;
};

} // namespace ebl