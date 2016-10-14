/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "drivers/vesa.h"

vscreen_t screen;

void setVScreen(uint16_t width, uint16_t height, uint16_t mode, uint16_t pitch, uint16_t bpp, uint32_t videoptr)
{
    screen.width = width;
    screen.height = height;
    screen.mode = mode;
    screen.pitch = pitch;
    screen.bpp = bpp;
    screen.framebuffer = videoptr;
}

uint32_t getPixelAddr(uint32_t x, uint32_t y)
{
    return (uint32_t)((y * screen.pitch) + (x * (screen.bpp / 8)) + screen.framebuffer);
}

void clearScreen()
{
    for(uint32_t y = screen.height + 1; y >= 0; y--)
    {
        for(uint32_t x = screen.width + 1; x >= 0; x--)
        {
            uint32_t *pixel = getPixelAddr(x, y);
            *pixel = 0x000080FF; //TODO: Don't assume RGB field sizes & positions
        }
    }
}