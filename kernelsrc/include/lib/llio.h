/*
 * File:   llio.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 10:54:04 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 29-Jul-2017 11:08:54 PM
*/

#pragma once

#include "system.h"
#include "arch.h"

#ifdef __cplusplus
extern "C" {
#endif

#if ARCH_TYPE == 0
  void io_wait();
  void outb(uint16_t port, uint8_t value);
  void outw(uint16_t port, uint16_t value);
  uint8_t inb(uint16_t port);
  uint16_t inw(uint16_t port);
  uint64_t read_cr0();
  uint64_t read_cr1();
  uint64_t read_cr2();
  uint64_t read_cr3();
  uint64_t read_cr4();
#elif ARCH_TYPE == 1
  
#endif

#ifdef __cplusplus
}
#endif
