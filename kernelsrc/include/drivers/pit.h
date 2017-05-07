/*
 * File:   pit.h
 * Author: Kevin
 *
 * Created on August 26, 2016, 8:18 AM
 */

#include "common.h"
#include "system/irq.h"

#ifndef PIT_H
#define PIT_H

#ifdef __cplusplus
extern "C" {
#endif

void init_timer(uint32_t frequency, irq_t handler);
void pit_delay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* PIT_H */
