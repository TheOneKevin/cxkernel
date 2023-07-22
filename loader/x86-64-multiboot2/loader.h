#pragma once

#include "multiboot2.h"
#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8

void console_init(int port);
void console_emit(char c);
void console_puts(char const* c);
void console_log(char const* c);
