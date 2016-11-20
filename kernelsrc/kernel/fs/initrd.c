/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "fs/initrd.h"
#include "memory/kheap.h"

KHEAPBM* kheap;
struct tar_header* filesPtr;
uint32_t ramdiskAddress;

uint32_t translateSize(const char *in)
{
    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;
    
    for(j = 11; j > 0; j--, count *= 8)
        size += ((in[j-1] - '0') * count);
    
    return size;
}

//Returns the number of files in the archive there are!
uint32_t getFileAmount(uint32_t address)
{
    uint32_t i;
    for (i = 0; ; i++) //We loop through each header
    {
        tar_header_t *header = (tar_header_t *)address;
        if (header -> filename[0] == '\0') //until we hit 0
            break;
 
        uint32_t size = translateSize(header -> size);
        address += ((size / 512) + 1) * 512;
        
        if (size % 512)
            address += 512;
    }
    
    return i;
}

//Returns the number of files in the archive there are!
void parse(uint32_t address)
{
    uint32_t i;
    for (i = 0; ; i++) //We loop through each header
    {
        tar_header_t *header = (tar_header_t *)address;
        if (header -> filename[0] == '\0') //until we hit 0
            break;
 
        uint32_t size = translateSize(header -> size);
        filesPtr[i] = *header;
        address += ((size / 512) + 1) * 512;
        
        if (size % 512)
            address += 512;
    }
}

void initInitrd(uint32_t addr)
{
    if(getFileAmount(addr) == 0)
    {
        kprintf("Initial ramdisk corrupted!");
        //TODO: Ramdisk corrupt protocol
    }
    else
    {
        addr = ramdiskAddress;
        filesPtr = (tar_header_t *)kmalloc(kheap, getFileAmount(ramdiskAddress) * sizeof(tar_header_t));
        parse(ramdiskAddress);
    }
}

void closeInitrd()
{
    kfree(kheap, filesPtr);
}

uint32_t findFile(char* fileName)
{
    uint32_t ret = ramdiskAddress;
    for(int i = 0; i < getFileAmount(ramdiskAddress); i++)
    {
        if(strcmp(filesPtr[i].filename, fileName) == 0)
        {
            return ret + 512;
        }
        
        uint32_t size = translateSize(filesPtr[i].size);
        ret += ((size / 512) + 1) * 512;
        if (size % 512)
            ret += 512;
    }
    
    return -1;
}