/*
 * File:   tty.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:12:20 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 04-Aug-2017 01:28:29 PM
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include "lib/buffers.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef void (*vHookA) ();
typedef void (*vHookB) (const char c);
typedef void (*vHookC) (uint8_t bg);

// This console is modelled after the VGA text mode console information
typedef struct
{
    uint16_t width;  // Screen width in number of characters
    uint16_t height; // Screen height in number of characters
    uint8_t _x; uint8_t _y; //Our current cursor positions

    // The current screen colours
    uint8_t background;
    uint8_t foreground;

    vHookA scroll;
    vHookA updc;
    vHookB putc;
    vHookC clear;

    bool isInTextmode; // Are we in text mode (for optimizations)

    void* VRAM;
    lbuffer_t* framebuffer;
} console_t;

// Keyboard baby!
typedef struct
{
    lbuffer_t* kbdBuffer;
} kbd_t;

typedef struct
{
    uint8_t     id;
    console_t* console;
    kbd_t*     keyboard;
} tty_t;

#ifdef __cplusplus
}
#endif
