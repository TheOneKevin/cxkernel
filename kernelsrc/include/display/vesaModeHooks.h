/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vesaModeHooks.h
 * Author: Kevin
 *
 * Created on October 15, 2016, 7:35 PM
 */

#include "common.h"

#ifndef VESAMODEHOOKS_H
#define VESAMODEHOOKS_H

#ifdef __cplusplus
extern "C" {
#endif

struct fontStruct
{
    uint32_t characterWidth;
    uint32_t characterHeight;
    uint32_t charX; //Amount of characters in the X axis
    uint32_t charY; //Amount of characters in the Y axis
}; typedef struct fontStruct font_t;

uint32_t queryWidth();
uint32_t queryHeight();

extern font_t currentfont;

void _iinitVesaConsole();
void printChar(uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool transparant, uint8_t c);

#ifdef __cplusplus
}
#endif

#endif /* VESAMODEHOOKS_H */

