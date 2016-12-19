#include "fs/vfs.h"
#include "fs/tar/tar.h"
#include "memory/kheap.h"
#include "system/kprintf.h"
#include "drivers/device.h"
#include "fs/initrd.h"
#include "exp_common.h"

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-parameter"

KHEAPBM* kheap;
mountpoint_t** mounts;

uint32_t _translateSize(const char* in)
{
    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;
    
    for(j = 11; j > 0; j--, count *= 8)
        size += ((in[j-1] - '0') * count);
    
    return size;
}

uint32_t _tar_getfilecount(uint32_t address)
{
    uint32_t i;
    for(i = 0; ; i++)
    {
        tar_header_t* header = (tar_header_t*)address;
        if(header -> filename[0] == '\0')
            break;
        
        uint32_t size = _translateSize(header -> size);
        address += ((size / 512) + 1) * 512;
        
        if (size % 512)
            address += 512;
    }
    
    return i;
}

void _tar_parse(uint32_t address, tar_header_t* nodes)
{
    uint32_t i;
    for (i = 0; ; i++) //We loop through each header
    {
        tar_header_t *header = (tar_header_t *)address;
        if (header -> filename[0] == '\0') //until we hit 0
            break;
 
        uint32_t size = _translateSize(header -> size);
        nodes[i] = *header;
        address += ((size / 512) + 1) * 512;
        
        if (size % 512)
            address += 512;
    }
}

uint32_t _tar_get_inode(tar_header_t* file, device_t* dev)
{
    tarpriv_t* p = dev -> fs -> private_data;
    for(uint32_t i = 0; i < p -> fileCount; i++)
    {
        if(strcmp(p -> nodes[i].filename, file -> filename) == 0)
            return i;
    }
    
    return -ENOENT;
}

tar_header_t* _tar_get_header(char* fileName, device_t* dev)
{
    initrdpriv_t* p = dev -> private_data;
    tarpriv_t* pp = dev -> fs -> private_data;
    
    uint32_t ret = p -> initrd_loc;
    for(uint32_t i = 0; i < pp -> fileCount; i++)
    {
        if(strcmp(pp -> nodes[i].filename, fileName) == 0)
        {
            return &pp -> nodes[i];
        }
        
        uint32_t size = _translateSize(pp -> nodes[i].size);
        ret += ((size / 512) + 1) * 512;
        if (size % 512)
            ret += 512;
    }
    
    return (tar_header_t*)-1;
}

status_t tar_readall(fsnode_t* file, uint32_t* buf, device_t* dev)
{
    initrdpriv_t* ipriv = dev -> private_data;
    tarpriv_t* tpriv = dev -> fs -> private_data;
    
    uint32_t ret = ipriv -> initrd_loc;
    for(uint32_t i = 0; i < tpriv -> fileCount; i++)
    {
        if(strcmp(tpriv -> nodes[i].filename, file -> locaPath) == 0)
        {
            uint32_t* ptr = (uint32_t*)(ret + 512); //Copy contents into buffer
            memcpy(buf, ptr, atoio(tpriv -> nodes[i].size) * sizeof(char));
            return 0;
        }
        
        uint32_t size = _translateSize(tpriv -> nodes[i].size);
        ret += ((size / 512) + 1) * 512;
        if (size % 512)
            ret += 512;
    }
    
    return -1;
}

status_t tar_read(fsnode_t* file, uint32_t* buf, uint32_t len, device_t* dev)
{
    return tar_readall(file, buf, dev);
}

bool tar_isdir(fsnode_t* a1)
{
    if(strcmp(a1 -> locaPath, "/") == 0) return true;
    return false;
}

status_t tar_write(fsnode_t* a1, uint32_t* a2, uint32_t a3, device_t* a4)
{
    return -EWRITE;
}

status_t tar_touch(char* a1, device_t* a2)
{
    return -EWRITE;
}

fsnode_t* tar_findnode(char* name, device_t* dev)
{
    tarpriv_t* priv = dev -> fs -> private_data;
    for(uint32_t i = 0; i < priv -> fileCount; i++)
    {
        if(strcmp(priv -> nodes[i].filename, name) == 0)
        {
            fsnode_t* rett = (fsnode_t*)kmalloc(kheap, sizeof(fsnode_t));
            rett -> locaPath = name;
            rett -> name     = priv -> nodes[i].filename;
            rett -> inode    = i;
            rett -> mountID  = dev -> mountId;
            rett -> isDir    = false;
            return rett;
        }
    }
    
    return 0;
}

status_t tar_stat(char* name, fstat_t* out, device_t* dev)
{
    if(!name || !out || !dev) return -EINVAL;
    fsnode_t* file = tar_findnode(name, dev);
    if(file == 0) return -ENOENT;
    tar_header_t* head = _tar_get_header(name, dev);
    out -> st_dev   = dev  -> id;
    out -> st_ino   = file -> inode;
    out -> st_mode  = atoio(head -> mode);
    out -> st_nlink = 1;
    out -> st_uid   = atoio(head -> uid);
    out -> st_gid   = atoio(head -> gid);
    out -> st_rdev  = 0;
    out -> st_size  = atoio(head -> size);
    out -> st_ctime = head -> mtime; //Both creation and modification are equal since
    out -> st_mtime = head -> mtime; //we cannot write to the init ram fs
    return 0;
}

bool tar_exists(char* name, device_t* dev)
{
    if(tar_findnode(name, dev) != 0) return true;
    return false;
}

status_t tar_mount(device_t* dev)
{
    if(!dev) return -EINVAL;
    tarpriv_t* priv = dev -> fs -> private_data;
    initrdpriv_t* priv1 = dev -> private_data;
    priv -> fileCount = _tar_getfilecount(priv1 -> initrd_loc); //Get amount of files
    tar_header_t* nodes = (tar_header_t*)kmalloc(kheap, sizeof(tar_header_t) * priv -> fileCount);
    priv -> nodes = nodes;
    _tar_parse(priv1 -> initrd_loc, priv -> nodes); //Add all nodes
    return 0;
}


uint32_t* tar_ls(char* dir, uint32_t* out, struct device* dev)
{
    tarpriv_t* priv = dev -> fs -> private_data;
    uint32_t* ret = (uint32_t*) kmalloc(kheap, sizeof(uint32_t) * priv -> fileCount);
    uint32_t i = 0;
    for(i = 0; i < priv -> fileCount; i++)
    {
        ret[i] = (uint32_t)tar_findnode(priv -> nodes[i].filename, dev);
    }
    *out = i;
    return ret;
}

status_t tar_probe(device_t* dev)
{
    //Initialize the tar file system
    dev -> fs = (filesystem_t*)kmalloc(kheap, sizeof(filesystem_t));
    dev -> fs -> name = "tar";
    
    dev -> fs -> probe     = tar_probe;
    dev -> fs -> readall   = tar_readall;
    dev -> fs -> read      = tar_read;
    dev -> fs -> isdir     = tar_isdir;
    dev -> fs -> touch     = tar_touch;
    dev -> fs -> write     = tar_write;
    dev -> fs -> stat      = tar_stat;
    dev -> fs -> exists    = tar_exists;
    dev -> fs -> findnode  = tar_findnode;
    dev -> fs -> mount     = tar_mount;
    dev -> fs -> ls        = tar_ls;
    
    dev -> fs -> private_data = (tarpriv_t*)kmalloc(kheap, sizeof(tarpriv_t));
    
    return 0;
}