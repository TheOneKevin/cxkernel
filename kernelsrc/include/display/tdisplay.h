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
enum text_color
{
    COLOR_BLACK =           0, //0x00000000
    COLOR_BLUE =            1, //0x000000FF
    COLOR_GREEN =           2, //0x0000FF00
    COLOR_CYAN =            3, //0x0000FFFF
    COLOR_RED =             4, //0x00FF0000
    COLOR_MAGENTA =         5, //0x00FF00FF
    COLOR_BROWN =           6, //0x00550000
    COLOR_LIGHT_GREY =      7, //0x00C0C0C0
    COLOR_DARK_GREY =       8, //0x00909090
    COLOR_LIGHT_BLUE =      9, //0x000055AA
    COLOR_LIGHT_GREEN =     10, //0x0000FF55
    COLOR_LIGHT_CYAN =      11, //0x002BEDFF
    COLOR_LIGHT_RED =       12, //0x00FF5555
    COLOR_LIGHT_MAGENTA =   13, //0x00FF55FF
    COLOR_YELLOW =          14, //0x00FFFF00
    COLOR_WHITE =           15, //0x00FFFFFF
};

struct screenInfo
{
    uint8_t _x; uint8_t _y; //Our current cursor positions
    uint8_t _lx; uint8_t _ly; //Lock x, lock y
    // The screen colours
    enum text_color background;
    enum text_color foreground;
}; typedef struct screenInfo screeninfo_t;

typedef void (*vHookA) (); //This is our putC hook!
typedef void (*vHookB) (const char c, bool isKey); //This is our putC hook!
typedef void (*vHookC) (enum text_color bg); //This is our putC hook!

extern screeninfo_t screen;

extern vHookA _scroll;
extern vHookA _updc;
extern vHookB _putc;
extern vHookC _clear;

//Initializes the console display
void console_init();
//Sets the console background to the specified colour
void console_setbg(enum text_color bg);
//Sets the console background to the specified colour
void console_setfg(enum text_color fg);
//Sets the console cursor to the specified positions
void set_cursorpos(uint8_t xpos, uint8_t ypos);
//Clears the console background to the specified colour
void console_clear();
//Writes a character on screen
void console_putc(const char c);
//Assumes the keyboard is printing to the screen
void console_putck(const char c);
//Writes a string on screen
void console_write(const char *c);
//Writes a string on screen with newline at the end
void console_writeline(const char *c);
//Writes a hexadecimal onto the screen
void console_write_hex(uint32_t n);
//Writes an unsigned integer onto the screen
void console_write_dec(uint32_t n);
//Writes a signed integer
void console_write_int(int n);
//Prints a string centered on screen
void console_print_center(const char *c);
//Print OK message
void bprintok();
//Print Error message
void bprinterr();
//Print Information message
void bprintinfo();
//Print Warning message
void bprintwarn();

#ifdef __cplusplus
}
#endif

#endif /* TDISPLAY_H */
