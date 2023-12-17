#pragma once

enum E {
    OK = 0,
    INVALID,
    NOT_IMPLEMENTED,
    INVALID_ARGUMENT,
    OUT_OF_MEMORY,
    PERMISSION_DENIED
};

struct status_t {
    E e;
    status_t(E e) : e{e} {}
    operator bool() const { return e == E::OK; }
};
