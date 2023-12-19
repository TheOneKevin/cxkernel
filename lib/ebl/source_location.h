#pragma once

#include <stdint.h>

// ref:
// https://github.com/llvm/llvm-project/blob/llvmorg-17.0.6/libcxx/include/source_location#L44

namespace std {
class source_location {
    // The names source_location::__impl, _M_file_name, _M_function_name, _M_line,
    // and _M_column are hard-coded in the compiler and must not be changed here.
    struct __impl {
        const char *_M_file_name;
        const char *_M_function_name;
        unsigned _M_line;
        unsigned _M_column;
    };
    const __impl *__ptr_ = nullptr;
    // GCC returns the type 'const void*' from the builtin, while clang returns
    // `const __impl*`. Per C++ [expr.const], casts from void* are not permitted
    // in constant evaluation, so we don't want to use `void*` as the argument
    // type unless the builtin returned that, anyhow, and the invalid cast is
    // unavoidable.
    using __bsl_ty = decltype(__builtin_source_location());

public:
    // The defaulted __ptr argument is necessary so that the builtin is evaluated
    // in the context of the caller. An explicit value should never be provided.
    static consteval source_location
    current(__bsl_ty __ptr = __builtin_source_location()) noexcept {
        source_location __sl;
        __sl.__ptr_ = static_cast<const __impl *>(__ptr);
        return __sl;
    }
    constexpr source_location() noexcept = default;

    constexpr uint32_t line() const noexcept {
        return __ptr_ != nullptr ? __ptr_->_M_line : 0;
    }
    constexpr uint32_t column() const noexcept {
        return __ptr_ != nullptr ? __ptr_->_M_column : 0;
    }
    constexpr const char *file_name() const noexcept {
        return __ptr_ != nullptr ? __ptr_->_M_file_name : "";
    }
    constexpr const char *function_name() const noexcept {
        return __ptr_ != nullptr ? __ptr_->_M_function_name : "";
    }
};
} // namespace std
