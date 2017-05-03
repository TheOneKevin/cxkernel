/* 
 * File:   exp_common.h
 * Author: kevin
 *
 * Created on November 30, 2016, 8:15 PM
 */

#include "common.h"

#ifndef EXP_COMMON_H
#define EXP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

// Converts an number into its string representation
char* iotoa(uint32_t);
// Converts a string representation of an integer into an integer
uint32_t atoio(char*);
char* convertToUnit(uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* EXP_COMMON_H */
