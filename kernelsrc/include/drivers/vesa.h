/*
 * File:   vesa.h
 * Author: Kevin
 *
 * Created on October 2, 2016, 6:45 PM
 */

#include "system.h"

#ifndef VESA_H
#define VESA_H

#ifdef __cplusplus
extern "C" {
#endif

// The lonely structs here probably won't be used for a while :/
/*
struct VbeInfoBlock
{
   char VbeSignature[4];             // == "VESA"
   uint16_t VbeVersion;              // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];         // isa vbeFarPtr
   uint8_t Capabilities[4];
   uint16_t VideoModePtr[2];         // isa vbeFarPtr
   uint16_t TotalMemory;             // as # of 64KB blocks
} __attribute__((packed));
typedef struct VbeInfoBlock vbe_info_struct_t;

struct VbeModeInfo
{
   uint16_t attributes;         // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
   uint8_t window_a;            // deprecated
   uint8_t window_b;            // deprecated
   uint16_t granularity;        // deprecated; used while calculating bank numbers
   uint16_t window_size;
   uint16_t segment_a;
   uint16_t segment_b;
   uint32_t win_func_ptr;       // deprecated; used to switch banks from protected mode without returning to real mode
   uint16_t pitch;              // number of bytes per horizontal line
   uint16_t width;              // width in pixels
   uint16_t height;             // height in pixels
   uint8_t w_char;              // unused...
   uint8_t y_char;              // ...
   uint8_t planes;
   uint8_t bpp;                 // bits per pixel in this mode
   uint8_t banks;               // deprecated; total number of banks in this mode
   uint8_t memory_model;
   uint8_t bank_size;           // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
   uint8_t image_pages;
   uint8_t reserved0;

   uint8_t red_mask;
   uint8_t red_position;
   uint8_t green_mask;
   uint8_t green_position;
   uint8_t blue_mask;
   uint8_t blue_position;
   uint8_t reserved_mask;
   uint8_t reserved_position;
   uint8_t direct_color_attributes;

   uint32_t framebuffer;           // physical address of the linear frame buffer; write here to draw to the screen
   uint32_t off_screen_mem_off;
   uint16_t off_screen_mem_size;   // size of memory in the framebuffer but not being displayed on the screen
   uint8_t reserved1[206];
} __attribute__ ((packed));
typedef struct VbeModeInfo vbe_mode_struct_t;
*/

struct VesaScreen
{
    uint16_t width;
    uint16_t height;
    uint16_t mode;
    uint16_t pitch;
    uint16_t bpp;
    uint32_t framebuffer;
    bool enabled;
};
typedef struct VesaScreen vscreen_t;

void setVScreen(uint16_t width, uint16_t height, uint16_t mode, uint16_t pitch, uint16_t bpp, uint32_t videoptr);
void setPixel(uint32_t x, uint32_t y, uint32_t c);
void clearScreen(uint32_t colour);
uint32_t getPixelAddr(uint32_t x, uint32_t y);
extern vscreen_t vhscreen;

extern uint32_t* vcache;

#ifdef __cplusplus
}
#endif

#endif /* VESA_H */
