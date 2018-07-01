/*
 * File:   arch_interface.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-03-28T14:29:31-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:34:24-04:00
*/

#pragma once

#include "common.h"
#include "core/console.h"

#ifdef __cplusplus
extern "C" {
#endif

console_t* arch_get_console(void);
void arch_early_init(uint32_t magic, void* ptr);
void arch_init(void);
void arch_late_init(void);
void arch_pmeminit(void);

#ifdef __cplusplus
}
#endif
