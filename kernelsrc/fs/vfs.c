#include "fs/vfs.h"
#include "drivers/device.h"
#include "display/tdisplay.h"
#include "memory/kheap.h"
#include "system/kprintf.h"

#define MAX_MOUNTS 16
mountpoint_t** mounts;
uint32_t mount_count;
KHEAPBM* kheap;

uint8_t _vfs_findmount(char* file)
{
    for(int i = 0; i < MAX_MOUNTS; i++)
    {
        char* f = (char*)kmalloc(kheap, sizeof(char));
        strcpy(f, file);
        char* s = mounts[i] -> mount;
        f[strlen(s)] = 0;
        if(strcmp(f, s) == 0)
        {
            kfree(kheap, f);
            return i;
        }
        
        kfree(kheap, f);
    }
    
    return 0;
}

fsnode_t* vfs_openfile(char* file)
{
    if(!file) return 0;
    if(!vfs_exists(file)) return 0;
    uint8_t i = _vfs_findmount(file);
    if(mounts[i] == 0 || mounts[i] -> dev == 0 || mounts[i] -> dev -> fs == 0) return 0;
    file += strlen(mounts[i] -> mount); //Convert to local
    fsnode_t* ret = mounts[i] -> dev -> fs -> findnode(file, mounts[i] -> dev);
    //We just assume the input file path is valid, given it was found
    ret -> fullpath = file - strlen(mounts[i] -> mount); //Then convert it back to global path from local
    return ret;
}

bool vfs_isdir(char* file)
{
    if(!file) return -EINVAL;
    if(!vfs_exists(file)) return -ENOENT;
    
    return vfs_openfile(file) -> isDir;
}

bool vfs_exists(char* file)
{
    if(!file) return false;
    uint8_t i = _vfs_findmount(file);
    if(mounts[i] == 0 || mounts[i] -> dev == 0 || mounts[i] -> dev -> fs == 0) return false;
    file += strlen(mounts[i] -> mount);
    if(mounts[i] -> dev -> fs -> exists(file, mounts[i] -> dev))
        return true;
    return false;
}

status_t vfs_write(char* file, uint32_t* buffer, uint32_t length, enum FileMode mode)
{
    if(file == 0 || buffer == 0 || length == 0) return -EINVAL;
    if(!vfs_exists(file)) return -ENOENT;
    if(mode == Write)
    {
        fsnode_t* f = vfs_openfile(file);
        if(mounts[f -> mountID] == 0 || mounts[f -> mountID] -> dev == 0
                || mounts[f -> mountID] -> dev -> fs == 0) return -EINVAL;
        if(!f -> isDir)
            return mounts[f -> mountID] -> dev -> fs ->
                    write(f, buffer, length, mounts[f -> mountID] -> dev);
    }
    else if(mode == WriteAll)
    {
        fsnode_t* f = vfs_openfile(file);
        if(mounts[f -> mountID] == 0 || mounts[f -> mountID] -> dev == 0
                || mounts[f -> mountID] -> dev -> fs == 0) return -EINVAL;
        if(!f -> isDir)
            return mounts[f -> mountID] -> dev -> fs ->
                    write(f, buffer, length, mounts[f -> mountID] -> dev);
    }
    
    return -EINVAL;
}

status_t vfs_read(char* file, uint32_t* buffer, enum FileMode mode)
{
    if(file == 0 || buffer == 0) return -EINVAL;
    if(!vfs_exists(file)) return -ENOENT;
    if(mode == Read)
    {
        fsnode_t* f = vfs_openfile(file);
        if(mounts[f -> mountID] == 0 || mounts[f -> mountID] -> dev == 0
                || mounts[f -> mountID] -> dev -> fs == 0) return -EINVAL;
        if(!f -> isDir)
            return mounts[f -> mountID] -> dev -> fs ->
                    readall(f, buffer, mounts[f -> mountID] -> dev);
    }
    
    return -EINVAL;
}

bool strbck(char c, char* in)
{
    int i = strlen(in) - 2;
    while(in[i] != c && i > 0)
        i--;
    in[i + 1] = '\0';
    if(i <= 1) return false;
    return true;
}

status_t vfs_ls(char* path)
{
    if(!path) return -EINVAL;
    mountpoint_t** mts; mts = (mountpoint_t**)kmalloc(kheap, sizeof(uint32_t) * mount_count);
    int j = 0;
    for(uint32_t i = 0; i < mount_count; i++)
    {
        char* m = (char*)kmalloc(kheap, sizeof(char)); strcpy(m, mounts[i] -> mount);
        while(1)
        {
            if(strbck('/', m))
            {
                if(strcmp(m, path) == 0)
                {
                    mts[j] = mounts[i];
                    j++;
                    break;
                }
            }
            
            else
            {
                if(strcmp(m, path) == 0)
                {
                    mts[j] = mounts[i];
                    j++;
                    break;
                }
                
                break;
            }
        }
        kfree(kheap, m);
    }
    
    for(int i = 0; i < j; i++)
    {
        uint32_t o = 0;
        uint32_t* s = mts[i] -> dev -> fs -> ls(path, &o, mts[i] -> dev);
        
        while(o > 0)
        {
            fsnode_t* fs = (fsnode_t*)s[o - 1];
            char* tmp = (char*)kmalloc(kheap, sizeof(char)); strcpy(tmp, mounts[i] -> mount);
            kprintf(" - %s\n", strcat(tmp, fs -> locaPath)); //Convert local path to file system path
            kfree(kheap, tmp); kfree(kheap, fs);
            o--;
        }
        
        kfree(kheap, s);
    }
    
    kfree(kheap, mts);
    j = 0;
    return 0;
}

status_t vfs_stat(fsnode_t* file, fstat_t* out)
{
    if(!file || !out) return -EINVAL;
    if(!vfs_exists(file -> fullpath)) return -ENOENT;
    mounts[file -> mountID] -> dev -> fs -> stat(file -> locaPath, out, mounts[file -> mountID] -> dev);
    return 0;
}

void vfs_init()
{
    mounts = (mountpoint_t**) kmalloc(kheap, sizeof(uint32_t) * MAX_MOUNTS);
    bprintok(); kprintf("VFS successfully initialized!\n");
}

struct device* vfs_list_mount()
{
    for(uint32_t i = 0; i < mount_count; i++)
    {
        kprintf(" - Device: %s | Path: %s\n", mounts[i] -> dev -> name, mounts[i] -> mount);
    }
    
    return 0;
}

status_t vfs_try_mount(struct device* dev, char* path)
{
    if(path[0] != '/') return -EINVAL; //ALL PATHS MUST START WITH the '/' character
    for(uint32_t i = 0; i < MAX_MOUNTS; i++)
    {
        if(strcmp(mounts[i] -> mount, path) == 0)
            return -EINVAL;
        if(mounts[i] == 0)
        {
            mountpoint_t* m = (mountpoint_t *)kmalloc(kheap, sizeof(mountpoint_t));
            m -> dev = dev;
            m -> mount = path;
            mounts[i] = m;
            dev -> mountId = i;
            mount_count++;
            return 0;
        }
    }
    
    return -EINVAL;
}

status_t vfs_remove_mount(uint32_t id)
{
    if(!id || id >= mount_count) return -EINVAL;
    if(mounts[id] == 0) return 0; //Prevent freeing non-existent ptr
    kfree(kheap, mounts[id]);
    mounts[id] = 0;
    return 0;
}