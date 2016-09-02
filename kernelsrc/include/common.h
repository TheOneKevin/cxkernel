/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   common.h
 * Author: Kevin
 *
 * Created on August 24, 2016, 4:55 PM
 */

// Every single header file should include this header :)
#include <stdbool.h> // C doesn't have booleans by default.
#include <stddef.h>
#include <stdint.h>

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

void     outb(uint16_t port, uint8_t value);
uint8_t  inb(uint16_t port);
uint16_t inw(uint16_t port);

void io_wait();

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define PAGE_SIZE 0x1000
#define PAGE_ALIGN 0xFFFFF000 //0x100000000 - PAGE_SIZE

uint64_t _length; uint64_t _addr; // Here we get the largest chunk of memory for paging

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

