/*
 * File:   acpi.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-23T14:29:22-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-18T09:55:15-04:00
*/

#pragma once

#include "common.h"
#include "acpitypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void initTmpBootACPI(void) NO_OPTIMIZE;
void deinitTmpBootACPI(void);
void getNUMADomains(void) NO_OPTIMIZE;
void getCPUInfo(void) NO_OPTIMIZE;
bool acpiHasAPIC(void) NO_OPTIMIZE;

#ifdef __cplusplus
}
#endif
