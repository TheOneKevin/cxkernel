#pragma once

#include <ebl/util.h>
#include <stdint.h>

namespace x86_64 {

   static inline void sti() {
      FENCE;
      asm volatile("sti");
   }

   static inline void cli() {
      asm volatile("cli");
      FENCE;
   }

   static inline uint64_t read_cr2() {
      uint64_t ret;
      asm volatile("mov %%cr2, %0" : "=r"(ret));
      return ret;
   }

   static inline uint64_t read_cr3() {
      uint64_t ret;
      asm volatile("mov %%cr3, %0" : "=r"(ret));
      return ret;
   }

   static inline uint32_t save_flags() {
      uint32_t state;
      asm volatile(
            "pushf;"
            "pop %0"
            : "=rm"(state)::"memory");
      return state;
   }

   static inline uint64_t read_gs_offset64(uintptr_t offset) {
      uint64_t ret;
      asm volatile("movq %%gs:%1, %0" : "=r"(ret) : "m"(*(uint64_t*)(offset)));
      return ret;
   }

   static inline void restore_flags(uint32_t flags) {
      asm volatile(
            "push %0;"
            "popf" ::"g"(flags)
            : "memory", "cc");
   }

   static inline void outb(uint16_t port, uint8_t value) {
      asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
   }

   static inline uint8_t inb(uint16_t port) {
      uint8_t ret;
      asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
      return ret;
   }

   enum class MSR : uint32_t { IA32_GS_BASE = 0xC0000101, IA32_KERNEL_GS_BASE = 0xC0000102 };

   static inline void wrmsr(MSR sel, uint64_t value) {
      uint32_t low = value & 0xFFFFFFFF;
      uint32_t high = value >> 32;
      asm volatile("wrmsr" : : "c"(sel), "a"(low), "d"(high));
   }

} // namespace x86_64
