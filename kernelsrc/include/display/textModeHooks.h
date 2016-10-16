/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   textModeHooks.h
 * Author: Kevin
 *
 * Created on October 15, 2016, 3:00 PM
 */

#ifndef TEXTMODEHOOKS_H
#define TEXTMODEHOOKS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Better not touch this (emergencies only, will make your code very un modular)
void iconsclear(enum text_color bg);
// Better not touch this too (emergencies only, will make your code very un modular)!
void putcraw(uint16_t *location, uint16_t entry);*/

//Initiate and install the print text functions
void _iinitNormalConsole();

#ifdef __cplusplus
}
#endif

#endif /* TEXTMODEHOOKS_H */

