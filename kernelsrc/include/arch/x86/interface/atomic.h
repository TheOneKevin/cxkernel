/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T22:30:00-04:00
 */

#pragma once

#include "common.h"

typedef struct { volatile int counter; } atomic_t;

#define ATOMIC_INIT(i) { (i) }
