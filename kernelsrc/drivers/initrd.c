/*
 * This is the code to parse the TAR format of our Initial Ram disk (initrd)
 */

#include "drivers/initrd.h"
#include "memory/kheap.h"
#include "system/kprintf.h"
#include "system/PANIC.h"
#include "display/tdisplay.h"
#include "fs/vfs.h"

size_t initrd_read(device_t* device, void* buffer, size_t length)
{
    initrdpriv_t* priv = device -> private_data;
    memcpy(buffer, (void*) priv -> initrd_loc + device -> pos, length);
    return length;
}

size_t initrd_write(device_t* device, void* buffer, size_t length)
{
    initrdpriv_t* priv = device -> private_data;
    memcpy((void*) priv -> initrd_loc + device -> pos, buffer, length);
    return length;
}

device_t* initInitrdDevice(uint32_t initrd_start, uint32_t initrd_end)
{
    if(!initrd_start || !initrd_end)
    {
        kprintf("Initial ramdisk corrupted/no ramdisk present!");
        //TODO: Ramdisk corrupt protocol
    }
    else
    {
        device_t* initrd_device = (device_t*) kmalloc(kheap, sizeof(device_t));
        initrd_device = (device_t*)kmalloc(kheap, sizeof(device_t));
        initrdpriv_t* initrd_priv = (initrdpriv_t*)kmalloc(kheap, sizeof(initrdpriv_t));
        initrd_priv -> initrd_loc = initrd_start;
        initrd_priv -> initrd_end = initrd_end;

        initrd_device -> name         = "initrd";
        initrd_device -> flags        = DEVICE_FLAG_NOWRITE | DEVICE_FLAG_BLOCK;
        initrd_device -> read         = initrd_read;
        initrd_device -> write        = initrd_write;
        initrd_device -> private_data = initrd_priv;

        bprintok(); kprintf("Initrd successfully initialized and loaded\n");
        return initrd_device;
    }

    return NULL;
}
