/*
 * This is the code to parse the TAR format of our Initial Ram disk (initrd)
 */

#include "fs/initrd.h"
#include "memory/kheap.h"
#include "system/kprintf.h"
#include "system/PANIC.h"
#include "display/tdisplay.h"
#include "drivers/device.h"
#include "fs/tar/tar.h"
#include "fs/vfs.h"

KHEAPBM* kheap;

//Open stream
void initInitrd(uint32_t initrd_start, uint32_t initrd_end)
{
    if(!initrd_start || !initrd_end)
    {
        kprintf("Initial ramdisk corrupted/no ramdisk present!");
        //TODO: Ramdisk corrupt protocol
    }
    else
    {
        device_t* initrd_device = (device_t*)kmalloc(kheap, sizeof(device_t));
        initrdpriv_t* initrd_priv = (initrdpriv_t*)kmalloc(kheap, sizeof(initrdpriv_t));
        initrd_priv -> initrd_loc = initrd_start;
        initrd_priv -> initrd_end = initrd_end;
        
        initrd_device -> id           = 42;
        initrd_device -> flags        = DEVICE_FLAG_NOWRITE | DEVICE_FLAG_BLOCK;
        initrd_device -> isValid      = true;
        initrd_device -> name         = "initrd";
        initrd_device -> fs           = 0;
        initrd_device -> read         = 0;
        initrd_device -> write        = 0;
        initrd_device -> private_data = initrd_priv;
        
        tar_probe(initrd_device);
        initrd_device -> fs -> mount(initrd_device);
        
        uint32_t out = vfs_try_mount(initrd_device, "/initrd/");
        if(out == 0) { bprintok(); kprintf("Initrd successfully initialized and loaded\n"); }
        else    { bprinterr();  kprintf("Uh, oh! Something happened to the initrd\n"); }
    }
}