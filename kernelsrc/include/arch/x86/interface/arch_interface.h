/*
 * File:   arch_interface.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T17:04:35-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:43:25-05:00
*/

#pragma once

#include "common.h"
#include "core/console.h"
#include "arch/x86/interface/meminit.h"

#ifdef __cplusplus
extern "C" {
#endif

console_t* arch_get_console(void);
void arch_early_init(uint32_t magic, void* ptr);
void arch_init(void);
void arch_late_init(void);

#ifdef __cplusplus
}
#endif
