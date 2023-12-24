#include "arch/interface.h"
#include "platform/interface.h"

void platform::init() {
   // Disable Intel 8259 PIC
   x86_64::outb(0xA1, 0xFF);
   x86_64::outb(0x21, 0xFF);
}
