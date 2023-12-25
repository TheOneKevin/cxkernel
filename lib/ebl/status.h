#pragma once

#include <ebl/assert.h>
#include <ebl/util.h>

#include "ebl/type_traits.h"

/**
 * @brief These error codes are used by the kernel to indicate the status of
 *        an operation. They are used instead of exceptions because C++
 *        exceptions are not supported in the kernel itself.
 */
enum E {
   // No error.
   OK = 0,

   // Generic error code.
   INVALID,

   // The requested operation is not implemented.
   NOT_IMPLEMENTED,

   // The caller passed an invalid argument to the function.
   INVALID_ARGUMENT,

   // Physical memory allocation failed. Most likely returned by the kernel
   // slab allocator. Will trigger the kernel paging process and attempt
   // to free up some memory.
   OUT_OF_MEMORY,

   // The caller does not have permission to perform the requested operation
   PERMISSION_DENIED,

   // Returned by VmRegion::* when virtual memory allocation fails. This is
   // technically a out-of-memory error, but we want to distinguish it from
   // the kernel heap running out of memory.
   ALLOCATION_FAILED
};

template <typename T>
struct Result {
public:
   Result(T&& v) : value_{ebl::move(v)}, error_{E::OK}, armed_{true} {}

   /* implicit */ Result(E e) : unused_{0}, error_{e}, armed_{true} {}

   ~Result() {
      value_.~T();
   }

   operator bool() { return this->status() == E::OK; }

   E status() {
      armed_ = false;
      return error_;
   }

   T unwrap() {
      if(armed_ == true) panic("Attempted to unwrap an armed Result");
      if(error_ != E::OK) panic("Attempted to unwrap an error Result");
      // Set these to avoid double unwrapping Result
      error_ = E::INVALID;
      armed_ = true;
      // Let go of our ownership of the value
      return ebl::move(value_);
   }

private:
   union {
      char unused_;
      T value_;
   };
   E error_;
   bool armed_;
};

template <>
struct Result<void> {
public:
   Result(E e) : error_{e}, armed_{true} {}
   operator bool() { return this->status() == E::OK; }
   E status() {
      armed_ = false;
      return error_;
   }
   ~Result() {
      if(armed_ == true) panic("Attempted to destroy an armed Result");
   }

private:
   E error_;
   bool armed_;
};
