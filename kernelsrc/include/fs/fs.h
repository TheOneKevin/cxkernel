/*
 * File:   fs.h
 * Author: kevin
 *
 * Created on June 25, 2017, 10:24 AM
 */

#include "system.h"

#ifndef FS_H
#define FS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t dev_t;
typedef uint32_t rdev_t;
typedef uint32_t mode_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef uint64_t nlink_t;
typedef uint64_t ino_t;
typedef uint64_t time_t;
typedef size_t   off_t;

#define FS_FILE        0b001
#define FS_DIRECTORY   0b010
#define FS_CHARDEVICE  0b011
#define FS_BLOCKDEVICE 0b100
#define FS_PIPE        0b101
#define FS_SYMLINK     0b110
#define FS_OTHER       0b111
#define FS_MOUNTPOINT  0b1000

#define PATH_SEPERATOR '/'

#define DEVICE_FLAG_BLOCK   1 // 1b
#define DEVICE_FLAG_NOWRITE 2 // 10b
#define DEVICE_FLAG_NOREAD  4 // 100b
#define DEVICE_FLAG_BROKEN  8 // 1000b

enum FileMode
{
    Open, Append, WriteAll, Write, Read, Lock
};

struct fsNode; typedef struct fsNode fsnode_t;
struct dirEnt; typedef struct dirEnt dirent_t;
struct fileSystem; typedef struct fileSystem filesystem_t;
struct vfsNode; typedef struct vfsNode vfsnode_t;

typedef struct device
{
    char*    name;
    uint8_t  flags;
    size_t (*read) (struct device *device, void *buffer, size_t length);
    size_t (*write)(struct device *device, void *buffer, size_t length);
    void* private_data;
    uint32_t pos;
} device_t;

//Based on Linux's <sys/stat.h>
struct fsStat
{
    dev_t    st_dev;    // Device ID containing the file
    ino_t    st_ino;    // File serial number
    mode_t   st_mode;   // Mode of file
    nlink_t  st_nlink;  // Number of links to the file
    uid_t    st_uid;    // User ID of the file
    gid_t    st_gid;    // Group ID of the file
    dev_t    st_rdev;   // Device ID if file is character of block special
    off_t    st_size;   // File size in bytes
    uint64_t st_ctime;  // Created time.
    uint64_t st_mtime;  // Modified time.
}; typedef struct fsStat fstat_t;

struct fsOps
{
    //File system operations to be filled by the file system driver:
    fsnode_t* (*touch)  (char*, fsnode_t* mount); // Creates an empty file with the path "name" using the driver (device)
    status_t  (*remove) (char*, fsnode_t* mount);
    fsnode_t* (*open)   (char*, fsnode_t* mount); // Opens a file from path and returns fsnode
    bool      (*exists) (char*, fsnode_t* mount); // Checks if the file path ("name") exists, reading with the driver (device)
};

struct fOps
{
    size_t    (*read)    (fsnode_t*, void*, size_t off, size_t len); // Read from n bytes from offset in file into buffer
    size_t    (*write)   (fsnode_t*, void*, size_t off, size_t len); // Write n bytes from offset in buffer to file
    status_t  (*close)   (fsnode_t*);
    dirent_t* (*readdir) (fsnode_t*, uint32_t);
    fsnode_t* (*finddir) (fsnode_t*, char* name);
};

struct fsNode
{
    char* name;
    filesystem_t* fs;

    uint32_t mask;
    uid_t    uid;
    gid_t    gid;
    uint32_t flags;
    ino_t    inode;
    uint32_t length;
    uint32_t impl;

    struct fOps* file_ops;

    fstat_t* file_stats; // File infos

    fsnode_t* ptr; // Used for symlinks and stuff
};

// POSIX Compliant
struct dirEnt
{
    char* name;
    uint32_t ino;
};

struct fileSystem
{
    char* name; // Name of this file system we are using
    void* private_data; // Some filesystem specific data
    struct fsOps* fs_ops;
    device_t* dev; // Not needed. Only if the FS applies to multiple devices that requires multiple drivers.
    fsnode_t* mount;
};

struct vfsNode
{
    char* name; // Mount location's parent folder name (very complicated stuff)
    char* path;
    fsnode_t* file; // This only becomes a mountpoint when file != NULL
    int refcount;
};

#ifdef __cplusplus
}
#endif

#endif /* FS_H */
