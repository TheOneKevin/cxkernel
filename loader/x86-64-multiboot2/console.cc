#include "loader.h"

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void console_init(int port) {
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); // (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void console_emit(char c) {
    while((inb(COM1+5) & 0x20) == 0);
    outb(COM1, c);
}

void console_puts(char const* c) {
    while(*c) console_emit(*c++);
}

void console_log(char const* c) {
    while(*c)
    {
        while((inb(COM2+5) & 0x20) == 0);
        outb(COM2, *c++);
    }
}
