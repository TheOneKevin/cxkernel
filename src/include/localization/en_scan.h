/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   en_scan.h
 * Author: Kevin
 *
 * Created on August 26, 2016, 10:24 AM
 */

#ifndef EN_SCAN_H
#define EN_SCAN_H

#ifdef __cplusplus
extern "C" {
#endif

enum EN_SCANCODES
{
    FIRST_NUMBER = 0x02,         //1
    LAST_NUMBER = 0x0B,          //0
    DASH = 0x0C,                 //-
    EQUALS = 0x0D,               //=
    FIRST_KEY_TOP_ROW = 0x10,    //q
    LAST_KEY_TOP_ROW = 0x1B,     //]
    FIRST_KEY_MIDDLE_ROW = 0x1E, //a
    LAST_KEY_MIDDLE_ROW = 0x2B,  /* \ */
    FIRST_KEY_BOTTOM_ROW = 0x2C, //z
    LAST_KEY_BOTTOM_ROW = 0x35,  // /
    LSHIFT = 0x2A, RSHIFT = 0x36, //The modifiers (shifts)
    SPACE = 0x39, ENTER = 0x1C,
    BCKSPACE = 0x0E, CAPS = 0x3A
};
char *_num = "1234567890";
char *_mnum = "!@#$%^&*()";
char *_dash = "-";
char *_mdash = "_";
char *_equals = "=";
char *_mequals = "+";
char *_top_row = "qwertyuiop[]";
char *_mtop_row = "QWERTYUIOP{}";
char *_middle_row = "asdfghjkl;'\\";
char *_mmiddle_row = "ASDFGHJKL:\"|";
char *_bottom_row = "zxcvbnm,./";
char *_mbottom_row = "ZXCVBNM<>?";

#ifdef __cplusplus
}
#endif

#endif /* EN_SCAN_H */

