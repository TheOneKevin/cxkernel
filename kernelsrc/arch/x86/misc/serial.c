/*
 * File:   serial.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 2017-08-27T13:34:28-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-11-26T12:48:24-05:00
*/

#include "arch/x86/llio.h"
#include "core/console.h"
#include "arch/x86/serial.h"

void init_serial(uint16_t port)
{
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    // (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(uint16_t port)
{
    return inb(port + 5) & 1;
}

char read_serial(uint16_t port)
{
    while (serial_received(port) == 0);
    return inb(port);
}

int is_transmit_empty(uint16_t port)
{
    return inb(port + 5) & 0x20;
}

void write_serial(uint16_t port, char a)
{
    while (is_transmit_empty(port) == 0);
    outb(port, a);
}

void init_serial_console(void)
{
    init_serial(COM1);
}

static char serial_console_read(void)
{
    return read_serial(COM1);
}

static void serial_console_write(char a)
{
    write_serial(COM1, a);
}

console_t __internal_serial_cons =
{
    .putc = serial_console_write,
    .getc = serial_console_read,
    .clear = 0
};
