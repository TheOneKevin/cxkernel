/*
 * File:   system.h
 * Author: Kevin
 *
 * Created on August 24, 2016, 4:55 PM
 */

#include "errno.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>

#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

// Every single header file should include this header :)
#include <stdbool.h> // C doesn't have booleans by default.

#define KRNLBASE 0xC0000000
#define INRDBASE 0xC1000000
#define FRAMEBSE 0xFF800000

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

extern void delay(uint32_t ms);

//Copy n = size amount of src into dst
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);
//Set n = size amount of value at buf
void* memset(void* bufptr, uint32_t value, size_t size);
//Compare n = size amount of a to b
int memcmp(const void *s1, const void *s2, size_t n);

//Gets the length of a string
size_t strlen(const char* str);
// Concatenate the NULL-terminated string src onto the end of dest and returns the concatenated version
char *strcat(char *dest, const char *src);
// Compares if s1 = s2
int strcmp(char* s1, char* s2);
// Copy the NULL-terminated string src into dest and returns dest
char *strcpy(char *dest, const char* src);

// Halts the cpu by going into an infinite loop
void halt();

// Our assembly (common) functions:

void io_wait();
void cli();
void sti();

unsigned long read_cr0();
unsigned long read_cr1();
unsigned long read_cr2();
unsigned long read_cr3();
unsigned long read_cr4();

void     outb(uint16_t port, uint8_t value);
void     outw(uint16_t port, uint16_t value);
uint8_t  inb (uint16_t port);
uint16_t inw (uint16_t port);

extern uint64_t _length; extern uint64_t _addr; // Here we get the largest chunk of memory for paging
extern bool doBootLog; uint8_t debugBuffer[4096];

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H */
