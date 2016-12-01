/*
 * Our highly experimental header with experimental stuff
 */
#include "exp_common.h" //Experimental

//This is HIGHLY experimental iotoa (int to string more specifically)
char* iotoa(uint32_t n)
{
    if (n == 0)
    {
        char* c2 = (char*)kmalloc(kheap, 32 * sizeof(char));
        *c2 = (char)'0';
        return c2;
    }

    int acc = n;
    char* c = (char*)kmalloc(kheap, 32);
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char* c2 = (char*)kmalloc(kheap, 32 * sizeof(char));
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    kfree(kheap, c);
    return c2;
}

char* convertToUnit(uint32_t input)
{
    if(input  >= 1073741824)
    {
        char* tmp = iotoa((uint32_t)input / 1073741824);
        char* res = strcat(tmp, " GB");
        kfree(kheap, tmp); //Gettin' down and dirty with memory management
        return res;
    }
    
    else if(input  >= 1048576)
    {
        char* tmp = iotoa((uint32_t)input / 1048576);
        char* res = strcat(tmp, " MB");
        kfree(kheap, tmp);
        return res;
    }
    
    else if(input  >= 1024)
    {
        char* tmp = iotoa((uint32_t)input / 1024);
        char* res = strcat(tmp, " KB");
        kfree(kheap, tmp);
        return res;
    }
    
    else
    {
        char* tmp = iotoa((uint32_t)input);
        char* res = strcat(tmp, " B");
        kfree(kheap, tmp);
        return res;
    }
}