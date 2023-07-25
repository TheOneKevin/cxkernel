#pragma once

enum E {
    SUCCESS = 0,
    INVALID,
    NOT_IMPLEMENTED
};

struct status_t {
    E e;
    status_t(E e) : e{e} {}
    operator bool() const { return e == E::SUCCESS; }
};
