
/* 
 * File:   vfs.h
 * Author: kevin
 *
 * Created on December 15, 2016, 6:58 AM
 */

#include "common.h"

#ifndef VFS_H
#define VFS_H

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
typedef size_t off_t;

//Based on Linux's <sys/stat.h>
struct fsStat
{
    dev_t   st_dev;   // Device ID containing the file
    ino_t   st_ino;   // File serial number
    mode_t  st_mode;  // Mode of file
    nlink_t st_nlink; // Number of links to the file
    uid_t   st_uid;   // User ID of the file
    gid_t   st_gid;   // Group ID of the file
    dev_t   st_rdev;  // Device ID if file is character of block special
    
    off_t st_size; // File size in bytes
    
    time_t st_ctime; //Created time
    time_t st_mtime; //Modified time
    
    /*blksize_t st_blksize a filesystem-specific preferred I/O block size for
                     this object.  In some filesystem types, this may
                     vary from file to file
    blkcnt_t  st_blocks  number of blocks allocated for this object*/
    
}; typedef struct fsStat fstat_t;

struct fsNode
{
    char* fullpath;   // Full path to file
    char* locaPath;   // Path within mount
    char* name;       // File name
    ino_t inode;      // inode number
    uint32_t mountID; // Mount number which contains the file
    
    bool isDir;
}; typedef struct fsNode fsnode_t;


struct mountPoint
{
    char* mount; //Mount location
    struct device* dev;
}; typedef struct mountPoint mountpoint_t;

enum FileMode
{
    Open, Append, WriteAll, Write, Read, Lock
};

struct fsDir
{
    char* name;
    fsnode_t* node;
}; typedef struct fsDir fsdir_t;

struct fileSystem
{
    char* name; //Name of this file system we are using
    
    //File system operations to be filled by the file system driver:
    status_t (*probe)    (struct device* dev); //Probe sets the device up for the filesystem, and sets the filesystem up for the device too
    status_t (*readall)  (fsnode_t* file, uint32_t* buf, struct device* dev); //Reads all the bytes from file into buffer using the driver (device)
    status_t (*read)     (fsnode_t* file, uint32_t* buf, uint32_t len, struct device* dev); //Reads n bytes from the file into buffer using the driver (device)
    bool     (*isdir)    (fsnode_t* file); //Check if the node is a directory or not
    status_t (*touch)    (char* name, struct device*); //Creates an empty file with the path "name" using the driver (device)
    status_t (*write)    (fsnode_t* file, uint32_t* buf, uint32_t len, struct device*); //Writes n bytes from buffer to file using driver (device)
    status_t (*stat)     (char* name, fstat_t*, struct device*); //Retrieves the file statistics (i.e., date, size etc.) using the driver (device)
    bool     (*exists)   (char* name, struct device*); //Checks if the file path ("name") exists, reading with the driver (device)
    fsnode_t*(*findnode) (char* name, struct device*); //Find the file node associated with the file path. Will not convert local path to file system path!
    status_t (*mount)    (struct device*); //Mount this file system onto device
    uint32_t*(*ls)       (char* dir, uint32_t* out, struct device* dev); //Gets a list of pointers to file nodes in a certain directory. KFREE NEEDED!
    
    void *private_data; //Some filesystem specific data
    
}; typedef struct fileSystem filesystem_t;

//Opens file. Returns file structure.
fsnode_t* vfs_openfile(char* file);
//Is this a directory?
bool vfs_isdir(char* file);
//Does this file/directory exist?
bool vfs_exists(char* file);
//Writes to file. KFREE NEEDED.
status_t vfs_write(char* file, uint32_t* buf, uint32_t len, enum FileMode mode);
//Reads from file
status_t vfs_read(char* file, uint32_t* buf, enum FileMode mode);
//Gets a list of files and subdirectories in directory
status_t vfs_ls(char* path);
//Gets VFS file/directory statistics
status_t vfs_stat(fsnode_t* file, fstat_t* out);

void vfs_init();
struct device* vfs_list_mount();
status_t vfs_try_mount(struct device* dev, char* path);
status_t vfs_remove_mount(uint32_t id);

extern mountpoint_t** mounts;

#ifdef __cplusplus
}
#endif

#endif /* VFS_H */
