/*
 * File:   sbrk.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Sunday, July 1st 2018, 1:04:55 pm
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-07-01T14:10:00-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

void* sbrk(ptrdiff_t n);
void sbrk_init(void);
