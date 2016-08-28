/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tdisplay.h
 * Author: Kevin
 *
 * Created on August 24, 2016, 5:36 PM
 */
#include "common.h"

#ifndef TDISPLAY_H
#define TDISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Hardware text mode color constants. */
enum text_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

//Sets the console background to the specified colour
void console_setbg(enum text_color bg);
//Sets the console background to the specified colour
void console_setfg(enum text_color fg);
//Sets the console cursor to the specified positions
void set_cursorpos(uint8_t xpos, uint8_t ypos);
//Clears the console background to the specified colour
void console_clear(enum text_color bg);
//Writes a character on screen
void console_putc(char c);
//Writes a string on screen
void console_write(char *c);
//Writes a string on screen with newline at the end
void console_writeline(char *c);
//Writes a hexadecimal onto the screen
void console_write_hex(uint32_t n);
//Writes a decimal onto the screen
void console_write_dec(uint32_t n);

void console_print_center(char *c);

#ifdef __cplusplus
}
#endif

#endif /* TDISPLAY_H */

