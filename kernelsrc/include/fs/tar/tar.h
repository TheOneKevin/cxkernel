
/* 
 * File:   tar.h
 * Author: kevin
 *
 * Created on December 17, 2016, 8:58 AM
 */
#include "common.h"

#ifndef TAR_H
#define TAR_H

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
}; typedef struct tar_header tar_header_t;



struct tarPrivate
{
    uint32_t fileCount;
    tar_header_t* nodes;
}; typedef struct tarPrivate tarpriv_t;

status_t tar_probe(struct device* dev);
uint32_t _translateSize(const char* in);
uint32_t _tar_getfilecount(uint32_t address);
tar_header_t* _tar_get_header(char* fileName, struct device* dev);

#ifdef __cplusplus
}
#endif

#endif /* TAR_H */

