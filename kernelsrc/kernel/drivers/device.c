#include "drivers/device.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"

#define MAX_DEV 16

device_t** devs;
uint32_t dev_count = 0;

status_t register_device(device_t* dev)
{
    if(!dev || !dev -> isValid) return -EINVAL;
    for(int i = 0; i < MAX_DEV; i++)
    {
        if(devs == 0)
        {
            devs[i] = dev;
            dev_count++;
            bprintinfo(); kprintf("Registered device: %u\n", i);
            return 0;
        }
    }
    
    return -ENODEV;
}