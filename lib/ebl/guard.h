#pragma once

#include <ebl/thread_safety.h>
#include <ebl/type_traits.h>

namespace ebl {

template<class T>
class SCOPED_CAPABILITY Guard final {
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
    explicit Guard(T* lock) ACQUIRE(lock) : impl_{Policy{lock}} {
        impl_.lock();
    }
    Guard(T* lock1, Guard<T>&& lock2) ACQUIRE(lock1) RELEASE(lock2) : impl_{Policy{lock1}} {
        lock2.release();
        impl_.lock();
    }
    void release() RELEASE() {
        impl_.unlock();
    }
    ~Guard() RELEASE() {
        release();
    }

private:
    Policy impl_;
};

} // namespace ebl