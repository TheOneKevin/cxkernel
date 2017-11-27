/*
 * File:   atomic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T21:20:55-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-30T22:22:01-04:00
*/

#pragma once

#include "config.h"

typedef struct { volatile int counter; } atomic_t;

#define ATOMIC_INIT(i) { (i) }
