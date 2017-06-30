/*
 * File:   initrd.h
 * Author: Kevin
 *
 * Created on October 16, 2016, 10:52 AM
 */

#include "system.h"
#include "fs/fs.h"

#ifndef INITRD_H
#define INITRD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t initrd_loc; //Location of the ram disk
    uint32_t initrd_end; //End of the ram disk
} initrdpriv_t;

device_t* initInitrdDevice(uint32_t initrd_start, uint32_t initrd_end);

#ifdef __cplusplus
}
#endif

#endif /* INITRD_H */
