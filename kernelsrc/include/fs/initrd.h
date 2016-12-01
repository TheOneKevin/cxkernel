/* 
 * File:   initrd.h
 * Author: Kevin
 *
 * Created on October 16, 2016, 10:52 AM
 */

#include "common.h"

#ifndef INITRD_H
#define INITRD_H

#ifdef __cplusplus
extern "C" {
#endif

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};
typedef struct tar_header tar_header_t;

extern struct tar_header* filesPtr;
extern uint32_t ramdiskAddress;

uint32_t translateSize(const char*);
uint32_t getFileAmount(uint32_t);
void parse(uint32_t);
void initInitrd(uint32_t);
void closeInitrd();
uint32_t findFile(char*);

#ifdef __cplusplus
}
#endif

#endif /* INITRD_H */
