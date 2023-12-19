#pragma once

#include "ebl/type_traits.h"
#include <ebl/util.h>
#include <ebl/assert.h>

enum E {
    OK = 0,
    INVALID,
    NOT_IMPLEMENTED,
    INVALID_ARGUMENT,
    OUT_OF_MEMORY,
    PERMISSION_DENIED
};

template<typename T>
struct Result {
public:
    Result(T&& v) : error_{E::OK}, armed_{true} {
        value_ = ebl::move(v);
    }

    /* implicit */ Result(E e) : unused_{0}, error_{e}, armed_{true} {}

    ~Result() {
        /* v::~T() */
    }

    operator bool() {
        return this->status() == E::OK;
    }

    E status() {
        armed_ = false;
        return error_;
    }

    T unwrap() {
        if(armed_ == false)
            panic("Attempted to unwrap an armed Result");
        if(error_ != E::OK)
            panic("Attempted to unwrap an error Result");
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

template<>
struct Result<void> {
public:
    Result(E e) : error_{e} {}
    operator bool() const { return error_ == E::OK; }
private:
    E error_;
};
