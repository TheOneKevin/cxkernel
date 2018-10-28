/*
 * File:   serial.cc
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 2017-08-27T13:34:28-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-11-26T12:48:24-05:00
*/

#include "arch/x86/llio.h"
#include "platform/pc/serial.h"

void pc_serial_init(uint16_t port)
{
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    // (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static int serial_received(uint16_t port)
{
    return inb(port + 5) & 1;
}

char pc_read_serial(uint16_t port)
{
    while (serial_received(port) == 0);
    return inb(port);
}

static int is_transmit_empty(uint16_t port)
{
    return inb(port + 5) & 0x20;
}

void pc_write_serial(uint16_t port, char a)
{
    while (is_transmit_empty(port) == 0);
    outb(port, a);
}
