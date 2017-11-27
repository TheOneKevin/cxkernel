/*
 * File:   acpi.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-23T14:29:22-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:38:35-05:00
*/

#pragma once

#include "common.h"
#include "acpitypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void initTmpBootACPI(void);
void deinitTmpBootACPI(void);
void getNUMADomains(void);
void getCPUInfo(void);
bool acpiHasAPIC(void);

#ifdef __cplusplus
}
#endif
