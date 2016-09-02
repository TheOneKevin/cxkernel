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
#include "system/tdisplay.h"

#ifndef PANIC_H
#define PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#ifdef __cplusplus
}
#endif

#endif /* PANIC_H */

