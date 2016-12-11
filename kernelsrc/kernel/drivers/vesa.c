/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "drivers/vesa.h"
#include "memory/kheap.h"

vscreen_t vhscreen;
KHEAPBM* kheap;
uint32_t* vcache;

void setVScreen(uint16_t width, uint16_t height, uint16_t mode, uint16_t pitch, uint16_t bpp, uint32_t videoptr)
{
    vhscreen.width = width;
    vhscreen.height = height;
    vhscreen.mode = mode;
    vhscreen.pitch = pitch;
    vhscreen.bpp = bpp;
    vhscreen.framebuffer = videoptr;
    
    vcache = (uint32_t *)kmalloc(kheap, sizeof(uint64_t) * width * height);
}

uint32_t getPixelAddr(uint32_t x, uint32_t y)
{
    return (uint32_t)((y * vhscreen.pitch) + (x * (vhscreen.bpp / 8)) + vhscreen.framebuffer);
}

void setPixel(uint32_t x, uint32_t y, uint32_t c)
{
    //Check if pixel is on the screen
    if(x <= vhscreen.width && y <= vhscreen.height)
    {
        uint32_t *pixel = (uint32_t *) getPixelAddr(x, y);
        *pixel = c;
        //Do the cache thing
        *(vcache + (uint32_t)(vhscreen.width * y + x)) = c;
    }
}

void clearScreen(uint32_t colour)
{
    for(uint32_t y = vhscreen.height + 1; y >= 1; y--)
    {
        for(uint32_t x = vhscreen.width + 1; x >= 1; x--)
        {
            uint32_t *pixel = (uint32_t *) getPixelAddr(x - 1, y - 1);
            *pixel = colour;
            *(vcache + (uint32_t)(vhscreen.width * y + x)) = colour;
        }
    }
}