/*
 * File:   vfs.h
 * Author: Kevin
 *
 * Created on December 15, 2016, 6:58 AM
*/

#include "system.h"
#include "fs/fs.h"

#ifndef VFS_H
#define VFS_H

#ifdef __cplusplus
extern "C" {
#endif

void vfs_init();

status_t vfs_try_mount(fsnode_t* s, char* d);
status_t vfs_remove_mount(char*);

fsnode_t* vfs_open    (char* path);
fsnode_t* vfs_touch   (char* path);
status_t  vfs_delete  (char* file);

void      vfs_close   (fsnode_t* file);
uint32_t  vfs_read    (fsnode_t* file, void* buf, uint32_t off, uint32_t len);
uint32_t  vfs_write   (fsnode_t* file, void* buf, uint32_t off, uint32_t len);
dirent_t* vfs_readdir (fsnode_t* file, uint32_t idx);
fsnode_t* vfs_finddir (fsnode_t* file, char* name);

extern vfsnode_t *fs_root;

#ifdef __cplusplus
}
#endif

#endif /* VFS_H */
