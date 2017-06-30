/*
 * File:   tar.h
 * Author: kevin
 *
 * Created on December 17, 2016, 8:58 AM
 */

#include "system.h"
#include "fs/fs.h"

#ifndef TAR_H
#define TAR_H

#ifdef __cplusplus
extern "C" {
#endif

#define TAR_NAME "tarfs"

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

void tarfs_init();
fsnode_t* tarfs_mount(device_t*);
void tarfs_unmount(filesystem_t*, device_t*);


#ifdef __cplusplus
}
#endif

#endif /* TAR_H */
