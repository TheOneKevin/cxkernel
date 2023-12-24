#include "arch/interface.h"
#include "platform/interface.h"

using namespace pc;
using namespace x86_64;

static void console_init_port(int port) {
   outb(port + 1, 0x00); // Disable all interrupts
   outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
   outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
   outb(port + 1, 0x00); // (hi byte)
   outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
   outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
   outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void platform::console_init() {
   console_init_port(COM1);
   console_init_port(COM2);
}

void platform::console_emit(char c) {
   while((inb(COM1 + 5) & 0x20) == 0)
      ;
   outb(COM1, c);
}

void platform::console_puts(char const* c) {
   while(*c) console_emit(*c++);
}

void platform::console_log(char const* c) {
   while(*c) {
      while((inb(COM2 + 5) & 0x20) == 0)
         ;
      outb(COM2, *c++);
   }
}
