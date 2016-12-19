/* 
 * File:   device.h
 * Author: kevin
 *
 * Created on December 14, 2016, 6:38 AM
 */

#include "common.h"

#ifndef DEVICE_H
#define DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEVICE_FLAG_BLOCK   1 // 1b
#define DEVICE_FLAG_NOWRITE 2 // 10b
#define DEVICE_FLAG_NOREAD  4 // 100b
#define DEVICE_FLAG_BROKEN  8 // 1000b
    
    
struct device
{
    uint32_t id;
    uint32_t mountId;
    uint8_t  flags;
    bool     isValid;
    char*    name;
    
    struct fileSystem *fs;
    
    status_t (*read) (struct device *device, uint32_t *buffer, uint32_t length, uint32_t st);
    status_t (*write)(struct device *device, uint32_t *buffer, uint32_t length, uint32_t st);
    
    void* private_data;
}; typedef struct device device_t;

status_t register_device(device_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_H */

