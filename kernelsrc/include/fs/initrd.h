/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

#ifdef __cplusplus
}
#endif

#endif /* INITRD_H */
