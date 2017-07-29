#include "memory/kheap.h"
#include "fs/vfs.h"
#include "lib/exp_system.h"
#include "fs/tar/tar.h"
#include "drivers/initrd.h"
#include "system/kprintf.h"

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-parameter"

struct fsOps* tarops = NULL;
struct fOps* fileops = NULL;

/* ========================= Internal Operations ========================= */

uint32_t _translateSize(const char* in)
{
    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

uint64_t _LtranslateSize(const char* in)
{
    uint64_t size = 0, j, count = 1;
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
    return size;
}

uint32_t _tar_getfilecount(filesystem_t* fs)
{
    uint32_t i;
    tar_header_t* header = (tar_header_t *) kmalloc(sizeof(tar_header_t));
    fs -> dev -> pos = 0; //Careful!
    for (i = 0;; i++)  //We loop through each header
    {
        fs -> dev -> read(fs -> dev, header, sizeof(tar_header_t));
        if (header -> filename[0] == '\0')
            break;

        uint32_t size = _translateSize(header -> size);
        fs -> dev -> pos += ((size / 512) + 1) * 512;

        if (size % 512)
            fs -> dev -> pos += 512;
    }

    kfree(header);
    return i;
}

void _tar_parse(filesystem_t* fs)
{
    tar_header_t* header = (tar_header_t *) kmalloc(sizeof(tar_header_t));
    tarpriv_t* priv = (tarpriv_t *) fs -> private_data;
    fs -> dev -> pos = 0;
    for (uint32_t i = 0;; i++)
    {
        fs -> dev -> read(fs -> dev, header, sizeof(tar_header_t));
        if (header -> filename[0] == '\0')
            break;

        uint32_t size = _translateSize(header -> size);
        priv -> nodes[i] = *header;
        fs -> dev -> pos += ((size / 512) + 1) * 512;

        if (size % 512)
            fs -> dev -> pos += 512;
    }

    kfree(header);
}

int _tar_get_inode(char* file, filesystem_t* fs)
{
    if (!fs)
        return -EINVAL;
    tarpriv_t* p = fs -> private_data;
    for (uint32_t i = 0; i < p -> fileCount; i++)
    {
        if (strcmp(p -> nodes[i] . filename, file) == 0)
            return i;
    }

    return -ENOENT;
}

tar_header_t* _tar_get_header(int inode, filesystem_t* fs)
{
    if (!fs)
        return NULL;
    tarpriv_t* pp = fs -> private_data;
    return &pp -> nodes[inode];
}

/* ========================= File Operations ========================= */

size_t tarfs_read(fsnode_t* file, void* buffer, size_t offset, size_t length)
{
    uint32_t ret = 0;
    tarpriv_t* tpriv = (tarpriv_t *) file -> fs -> private_data;
    for (uint32_t i = 0; i < file -> inode; i++)
    {
        uint32_t size = _translateSize(tpriv -> nodes[i] . size);
        ret += ((size / 512) + 1) * 512;
        if (size % 512)
            ret += 512;
    }

    file -> fs -> dev -> pos = ret + 512 + offset;
    file -> fs -> dev -> read(file -> fs -> dev, buffer, length);

    return atoio(tpriv -> nodes[file -> inode] . size) * sizeof(char);
}

size_t tarfs_write(fsnode_t* file, void* buffer, size_t offset, size_t length)
{
    return 0; // We cannot write to the tarfs (readonly filesystem)
}

status_t tarfs_close(fsnode_t* file)
{
    kfree(file -> name);
    kfree(file -> file_stats);
    kfree(file);
    return 0;
}

dirent_t* tarfs_readdir(fsnode_t* file, uint32_t idx)
{
    return NULL;
}

fsnode_t* tarfs_finddir(fsnode_t* parent, char* name)
{
    return NULL;
}

status_t tarfs_stat(fsnode_t* file, fstat_t* out)
{
    if (!file || !out)
        return -EINVAL;
    tar_header_t* head = _tar_get_header(file -> inode, file -> fs);
    out -> st_dev = 0;   // Wut?
    out -> st_ino = file -> inode;
    out -> st_mode = atoio(head -> mode);
    out -> st_nlink = 1;
    out -> st_uid = atoio(head -> uid);
    out -> st_gid = atoio(head -> gid);
    out -> st_rdev = 0;
    out -> st_size = _LtranslateSize(head -> size);   //Octal to decimal
    out -> st_ctime = _LtranslateSize(head -> mtime); //Both creation and modification are equal since
    out -> st_mtime = _LtranslateSize(head -> mtime); //we cannot write to the init ram fs
    return 0;
}

/* ========================= Filesystem Operations ========================= */

fsnode_t* tarfs_touch(char* path, fsnode_t* mount)
{
    return 0; //Readonly file system
}

fsnode_t* tarfs_open(char* path, fsnode_t* mount)
{
    fsnode_t* ret = NULL;
    tarpriv_t* tpriv = mount -> fs -> private_data;

    for (uint32_t i = 0; i < tpriv -> fileCount; i++)
    {
        if (strcmp(tpriv -> nodes[i] . filename, path + 1) == 0)  //path + 1 to ignore the prepended '/'
        {
            ret = (fsnode_t *) kmalloc(sizeof(fsnode_t));
            ret -> name = strdup(path); // The only thing we need to free
            ret -> fs = mount -> fs;    // All files should have a pointer to the filesystem of the mount
            ret -> inode = i;
            ret -> length = atoio(tpriv -> nodes[i] . size) * sizeof(char);
            ret -> file_ops = fileops;
            ret -> file_stats = (fstat_t *) kmalloc(sizeof(fstat_t));
            tarfs_stat(ret, ret -> file_stats);
            return ret;
        }
    }

    return ret;
}

bool tarfs_exists(char* path, fsnode_t* mount)
{
    tarpriv_t* tpriv = mount -> fs -> private_data;
    for (uint32_t i = 0; i < tpriv -> fileCount; i++)
    {
        if (strcmp(tpriv -> nodes[i] . filename, path + 1) == 0)  //path + 1 to ignore the prepended '/'
            return true;
    }

    return false;
}

void tarfs_init()
{
    if (!tarops)
    {
        tarops = (struct fsOps*) kmalloc(sizeof(struct fsOps));
        tarops  -> exists = tarfs_exists;
        tarops  -> open = tarfs_open;
        tarops  -> touch = tarfs_touch;
    }

    if (!fileops)
    {
        fileops = (struct fOps*)  kmalloc(sizeof(struct fOps));
        fileops -> close = tarfs_close;
        fileops -> finddir = tarfs_finddir;
        fileops -> read = tarfs_read;
        fileops -> readdir = tarfs_readdir;
        fileops -> write = tarfs_write;
    }
}

fsnode_t* tarfs_mount(device_t* dev)
{
    filesystem_t* fs = (filesystem_t *) kmalloc(sizeof(filesystem_t));
    tarpriv_t* priv = (tarpriv_t *) kmalloc(sizeof(tarpriv_t));
    fsnode_t* mount = (fsnode_t *) kmalloc(sizeof(fsnode_t));

    fs -> dev = dev;
    fs -> fs_ops = tarops;
    fs -> name = TAR_NAME;
    fs -> private_data = priv;
    fs -> mount = mount;

    priv -> nodes = (tar_header_t *) kmalloc(_tar_getfilecount(fs) * sizeof(tar_header_t));
    priv -> fileCount = _tar_getfilecount(fs);
    _tar_parse(fs);

    mount -> name = TAR_NAME;
    mount -> file_ops = fileops;
    mount -> fs = fs;
    mount -> flags = FS_MOUNTPOINT | FS_DIRECTORY;

    return mount;
}

void tarfs_unmount(filesystem_t* fs, device_t* dev)
{
    kfree(((tarpriv_t *) fs -> private_data) -> nodes);
    kfree(fs -> private_data);
}
