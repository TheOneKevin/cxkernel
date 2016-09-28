/*
 * To change this license header; choose License Headers in Project Properties.
 * To change this template file; choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cpufeatset.h
 * Author: Kevin
 *
 * Created on September 18, 2016, 8:41 AM
 */

#ifndef CPUFEATSET_H
#define CPUFEATSET_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t isIntel;

extern bool _FPU;
extern bool _VME;
extern bool _PSE;
extern bool _PAE;
extern bool _APIC;
extern bool _ACPI;
extern bool _MTRR;
extern bool _HTT;
extern bool _TM1;

extern bool _TSC;
extern bool _MSR;
extern bool _SSE;
extern bool _SSE2;
extern bool _SSE3;
extern bool _SSSE3;
extern bool _SSE4_1;
extern bool _SSE4_2;
extern bool _AVX;
extern bool _F16C;
extern bool _RDRAND;

extern bool _IA64;

#ifdef __cplusplus
}
#endif

#endif /* CPUFEATSET_H */

