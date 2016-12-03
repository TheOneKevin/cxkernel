/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pmm.h
 * Author: Kevin
 *
 * Created on September 24, 2016, 1:52 PM
 */

#include "common.h"

#ifndef PMM_H
#define PMM_H

#ifdef __cplusplus
extern "C" {
#endif

void initPmm();
uint32_t allocFrame();
void freeFrame(uint32_t a);
uint32_t pageAlign(uint32_t addr);

extern uint32_t framestart;
uint32_t* startheap;

#ifdef __cplusplus
}
#endif

#endif /* PMM_H */

