#pragma once

#include "ebl/util.h"

// Implement typesafe C++ atomics on integral types
// A lot of the functions are re-implementations from libcxx
// ref: https://github.com/llvm-mirror/libcxx

namespace ebl {
    enum memory_order : int {
        memory_order_relaxed = __ATOMIC_RELAXED,
        memory_order_acquire = __ATOMIC_ACQUIRE,
        memory_order_release = __ATOMIC_RELEASE,
        memory_order_acq_rel = __ATOMIC_ACQ_REL,
        memory_order_seq_cst = __ATOMIC_SEQ_CST,
    };

    template<typename T>
    class atomic {
        static_assert(is_integral_v<T> && !is_same_v<T, bool>,
            "Atomic only supports integral types (except bool)");
    public:
        void store(T desr, memory_order m = memory_order_seq_cst) volatile noexcept {
            __atomic_store_n(&value_, desr, m);
        }
        void store(T desr, memory_order m = memory_order_seq_cst) noexcept {
            __atomic_store_n(&value_, desr, m);
        }
        T load(memory_order m = memory_order_seq_cst) const volatile noexcept {
            return __atomic_load_n(&value_, m);
        }
        T load(memory_order m = memory_order_seq_cst) const noexcept {
            return __atomic_load_n(&value_, m);
        }
        operator T() const volatile noexcept { return load(); }
        operator T() const noexcept { return load(); }
        T exchange(T desr, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_exchange_n(&value_, desr, m);
        }
        T exchange(T desr, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_exchange_n(&value_, desr, m);
        }
        bool compare_exchange_weak(
            T& expc, T desr, memory_order s, memory_order f) volatile noexcept
        {
            return __atomic_compare_exchange_n(&value_, &expc, desr, true, s, f);
        }
        bool compare_exchange_weak(T& expc, T desr, memory_order s, memory_order f) noexcept {
            return __atomic_compare_exchange_n(&value_, &expc, desr, true, s, f);
        }
        bool compare_exchange_strong(
            T& expc, T desr, memory_order s, memory_order f) volatile noexcept
        {
            return __atomic_compare_exchange_n(&value_, &expc, desr, false, s, f);
        }
        bool compare_exchange_strong(
            T& expc, T desr, memory_order s, memory_order f) noexcept
        {
            return __atomic_compare_exchange_n(&value_, &expc, desr, false, s, f);
        }
        T fetch_add(T op, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_add(&value_, op, m);
        }
        T fetch_add(T op, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_fetch_add(&value_, op, m);
        }
        T fetch_sub(T op, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_sub(&value_, op, m);
        }
        T fetch_sub(T op, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_fetch_sub(&value_, op, m);
        }
        T fetch_and(T op, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_and(&value_, op, m);
        }
        T fetch_and(T op, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_fetch_and(&value_, op, m);
        }
        T fetch_or(T op, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_or(&value_, op, m);
        }
        T fetch_or(T op, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_fetch_or(&value_, op, m);
        }
        T fetch_xor(T op, memory_order m = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_xor(&value_, op, m);
        }
        T fetch_xor(T op, memory_order m = memory_order_seq_cst) noexcept {
            return __atomic_fetch_xor(&value_, op, m);
        }
    public:
        atomic() noexcept = default;
        atomic(T value) noexcept : value_{value} { }
        DELETE_COPY(atomic);
        DELETE_MOVE(atomic);
        atomic& operator=(const atomic&) volatile = delete;
        atomic& operator=(atomic&&) volatile = delete;

    private:
        T value_;
    };

    inline void atomic_thread_fence(memory_order m) noexcept {
        __atomic_thread_fence(m);
    }

    inline void atomic_signal_fence(memory_order m) noexcept {
        __atomic_signal_fence(m);
    }
} // namespace ebl
