/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "display/vesaModeHooks.h"
#include "drivers/vesa.h"

font_t currentfont;

#include "display/font8x8.h"

void resizePixels(uint32_t pixels[], uint32_t w1, uint32_t h1, uint32_t w2, uint32_t h2, uint32_t returnA[])
{
    int temp[w2 * h2];
    // EDIT: added +1 to account for an early rounding problem
    int x_ratio = (int)((w1<<16)/w2) +1;
    int y_ratio = (int)((h1<<16)/h2) +1;
    int x2, y2 ;
    for (int i = 0; i < h2; i++)
    {
        for (int j = 0; j < w2; j++)
        {
            x2 = ((j*x_ratio)>>16);
            y2 = ((i*y_ratio)>>16);
            temp[(i * w2) + j] = pixels[(y2 * w1) + x2] ;
        }                
    }                
    returnA = temp;
}

void printChar(uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, bool transparant, char c)
{
    //uint32_t array[8];
    //resizePixels(font8x8_basic[c][0], 8, 8, currentfont.characterWidth, currentfont.characterHeight, array[]);
    for(int iy = 0; iy < 8; iy++) //Loop through each entry of the character
    {
        char charc = font8x8_basic[c][iy]; //iy will be the pixel set on the y axis of the character (row #)
        for(int ix = 0; ix < 8; ix++) //Loop through each bit of that entry
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