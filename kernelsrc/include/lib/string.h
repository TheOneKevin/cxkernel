/**
 * @file   string.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T15:08:40-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compares the first @p len bytes of @p buf1 to @p buf2
 * Will return 0 if they match, and a non zero integer if they don't
 * 
 * @param buf1 Pointer to first buffer
 * @param buf2 Pointer to second buffer
 * @param len  Number of bytes to compare
 * 
 * @return 0 if contents of both memory blocks are equal
 * | Value | Description |
 * |-------|-------------|
 * | < 0   | If the first byte that does not match in both memory blocks has a lower value in @p buf1 than in @p buf2 (if evaluated as unsigned char values)   |
 * | = 0   | If the contents of both memory blocks are equal |
 * | > 0   | If the first byte that does not match in both memory blocks has a greater value in @p buf1 than in @p buf2 (if evaluated as unsigned char values) |
 * 
 */
int memcmp(const void* buf1, const void* buf2, size_t len);

/**
 * Copies @p len bytes from @p src into @p dest
 * 
 * @param dest Pointer to destination buffer
 * @param src  Pointer to source buffer
 * @param len  Number of bytes to copy
 * @return Pointer to destination buffer. Will return back @p dest.
 */
void* memcpy(void* dest, const void* src, size_t len);

/**
 * 
 * @param str 
 * @param c 
 * @param len 
 * @return void* 
 */
void* memset(void* str, int c, size_t len);

/**
 * 
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcat(char* dest, const char* src);

/**
 * 
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcpy(char* dest, const char* src);

/**
 * 
 * @param str 
 * @return char* 
 */
char* strdup(const char* str);

/**
 * 
 * @param str1 
 * @param str2 
 * @return int 
 */
int strcmp(const char* str1, const char* str2);

/**
 * 
 * @param str 
 * @return size_t 
 */
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif
