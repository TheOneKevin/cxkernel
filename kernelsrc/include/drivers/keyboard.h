/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   keyboard.h
 * Author: Kevin
 *
 * Created on August 26, 2016, 9:05 AM
 */

#include "common.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

void register_keyboard();
void noDisplay(bool yesno);
void flush_cache();

uint8_t scan_to_ascii(uint8_t key);

typedef void (*keyH) (uint8_t scancode);

void installKeyHandler(keyH handler, int number);
void uninstallKeyHandler(int number);
void setHandlerFlag(int handler);

#ifdef __cplusplus
}
#endif

#endif /* KEYBOARD_H */

