/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "drivers/vesa.h"

vscreen_t vhscreen;

void setVScreen(uint16_t width, uint16_t height, uint16_t mode, uint16_t pitch, uint16_t bpp, uint32_t videoptr)
{
    vhscreen.width = width;
    vhscreen.height = height;
    vhscreen.mode = mode;
    vhscreen.pitch = pitch;
    vhscreen.bpp = bpp;
    vhscreen.framebuffer = videoptr;
}

uint32_t getPixelAddr(uint32_t x, uint32_t y)
{
    return (uint32_t)((y * vhscreen.pitch) + (x * (vhscreen.bpp / 8)) + vhscreen.framebuffer);
}

void setPixel(uint32_t x, uint32_t y, uint32_t c)
{
    uint32_t *pixel = (uint32_t *) getPixelAddr(x, y);
    *pixel = c;
}

void clearScreen(uint32_t colour)
{
    for(uint32_t y = vhscreen.height + 1; y >= 1; y--)
    {
        for(uint32_t x = vhscreen.width + 1; x >= 1; x--)
        {
            uint32_t *pixel = (uint32_t *) getPixelAddr(x - 1, y - 1);
            *pixel = colour;
        }
    }
}