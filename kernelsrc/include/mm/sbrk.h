/**
 * @file   sbrk.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, July 1st 2018, 1:04:55 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:44:50-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

/**
 * Increases the kernel break line by an amount n. A positive
 * increase will result in allocation, and a negative amount will
 * result in deallocation. An allocation will occur in chunks
 * of size ARCH_PAGE_SIZE. An increase of 0 will return the current
 * break address.
 * 
 * @param n  Amount to increase break by
 * @return   Pointer to break before increase
 */ 
void* sbrk(ptrdiff_t n);

void sbrk_init(void);
