#pragma once

#include <stdint.h>

namespace pc {

constexpr uint16_t COM1  = 0x3F8;
constexpr uint16_t COM2  = 0x2F8;

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

} // namespace pc
