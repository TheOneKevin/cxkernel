/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   kprintf.h
 * Author: Kevin
 *
 * Created on September 3, 2016, 10:57 PM
 */

#include "common.h"

#ifndef KPRINTF_H
#define KPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

//static bool kprint(const char* data, size_t length);
int kputc(int ic);
int kprintf(const char* restrict format, ...);

#ifdef __cplusplus
}
#endif

#endif /* KPRINTF_H */