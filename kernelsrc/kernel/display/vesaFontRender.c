/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "display/vesaModeHooks.h"
#include "drivers/vesa.h"

font_t currentfont;

#include "display/font8x8.h"

void printChar(uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool transparant, uint8_t c)
{
    for(uint8_t iy = 0; iy < 8; iy++) //Loop through each entry of the character
    {
        uint8_t charc = font8x8_basic[c][iy]; //iy will be the pixel set on the y axis of the character (row #)
        for(uint8_t ix = 0; ix < 8; ix++) //Loop through each bit of that entry
        {
            bool bit = (charc & (1 << ix)) != 0; //ix will be the pixel set on the x axis
            if(bit) //If it's a one, set a pixel at ix, iy
            {
                setPixel(ix + x * currentfont.characterWidth, iy + y * currentfont.characterHeight, fg);
            }
            else if(!transparant)
            {
                setPixel(ix + x * currentfont.characterWidth, iy + y * currentfont.characterHeight, bg);
            }
        }
    }
}