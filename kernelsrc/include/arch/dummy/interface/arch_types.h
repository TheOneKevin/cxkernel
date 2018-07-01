/*
 * File:   arch_types.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-03-28T14:30:45-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:33:04-04:00
*/

#pragma once

#include "common.h"

/**
 * Change according to the address space you're dealing with
 * i.e., change to 64 bit if dealing with 64 bit address space
 */

typedef uint32_t virt_t;
typedef uint32_t phys_t;
