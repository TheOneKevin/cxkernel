/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   acpi.h
 * Author: Kevin
 *
 * Created on September 2, 2016, 5:09 PM
 */

#include "common.h"

#ifndef ACPI_H
#define ACPI_H

#ifdef __cplusplus
extern "C" {
#endif

void acpiPowerOff(void);
int initAcpi(void);
int acpiEnable(void);
void acpiDisable(void);
void reboot();
int acpiCheckHeader(unsigned int *ptr, char *sig);
unsigned int *acpiCheckRSDPtr(unsigned int *ptr);
unsigned int *acpiGetRSDPtr(void);

#ifdef __cplusplus
}
#endif

#endif /* ACPI_H */

