/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PANIC.h
 * Author: Kevin
 *
 * Created on September 2, 2016, 6:08 PM
 */
#include "common.h"

#ifndef PANIC_H
#define PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

void panic(const char *message);
void assert(bool assertStatus, const char *message);
#define PANIC panic
#define ASSERT assert

#ifdef __cplusplus
}
#endif

#endif /* PANIC_H */