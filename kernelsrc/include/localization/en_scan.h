/* 
 * File:   en_scan.h
 * Author: Kevin
 *
 * Created on August 26, 2016, 10:24 AM
 */
#include "localization/scanmap.h"

#ifndef EN_SCAN_H
#define EN_SCAN_H

#ifdef __cplusplus
extern "C" {
#endif

// These are the characters corresponding to each scan code range. Note that these may not
// appear in order on your keyboard. Please refer to a scan code map to avoid errors in
// remapping/mapping

char *_num = "1234567890";
char *_mnum = "!@#$%^&*()";

char *_dash = "-";
char *_mdash = "_";

char *_equals = "=";
char *_mequals = "+";

char *_forward = "\\"; char *_mforward = "|";

char *_top_chars = "[]"; char *_mtop_chars = "{}";              //Corresponds to range TC_S -> TC_E
char *_middle_chars = ";'`"; char *_mmiddle_chars = ":\"~";     //Corresponds to range MC_S -> MC_E
char *_bottom_chars = ",./"; char *_mbottom_chars = "<>?";      //Corresponds to range BC_S -> BC_E

char *_top_row = "qwertyuiop";
char *_mtop_row = "QWERTYUIOP";

char *_middle_row = "asdfghjkl";
char *_mmiddle_row = "ASDFGHJKL";

char *_bottom_row = "zxcvbnm";
char *_mbottom_row = "ZXCVBNM";

#ifdef __cplusplus
}
#endif

#endif /* EN_SCAN_H */
