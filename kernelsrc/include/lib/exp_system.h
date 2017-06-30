/*
 * File:   exp_common.h
 * Author: kevin
 *
 * Created on November 30, 2016, 8:15 PM
 */

#include "system.h"

#ifndef EXP_SYSTEM_H
#define EXP_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

// Converts an number into its string representation
char* iotoa(uint32_t);
// Converts a string representation of an integer into an integer
uint32_t atoio(char*);
char* convertToUnit(uint32_t);
char* pad(char* in, char c, uint32_t amount, bool isLeft);
char* strdup(const char* in);

#ifdef __cplusplus
}
#endif

#endif /* EXP_SYSTEM_H */
