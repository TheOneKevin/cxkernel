/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, March 28th 2018, 2:29:31 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T22:29:34-04:00
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
