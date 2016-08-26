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

/* Copy "len" bytes from "src" to "dest".
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
// Write "len" copies of "val' into "dest".
void memset(uint8_t *dest, uint8_t val, uint32_t len);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);*/

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

