/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, March 28th 2018, 2:32:10 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T22:29:43-04:00
 */

#pragma once

#include "common.h"

typedef struct { volatile int counter; } atomic_t;
#define ATOMIC_INIT(i) (i)
