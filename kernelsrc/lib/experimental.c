/*
 * Our highly experimental header with experimental stuff
 */

#include "lib/exp_system.h" //Experimental
#include "memory/kheap.h"

//This is HIGHLY experimental iotoa (int to string more specifically)
char* iotoa (uint32_t n)
{
    if (n == 0)
    {
        char* c2 = (char*)kmalloc(32 * sizeof(char));
        *c2 = (char)'0';
        return c2;
    }

    int acc = n;
    char* c = (char*)kmalloc(32);
    int i = 0;
    while (acc > 0)
    {
        c [i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c [i] = 0;

    char* c2 = (char*)kmalloc(32 * sizeof(char));
    c2 [i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2 [i--] = c [j++];
    }
    kfree(c);
    return c2;
}

char* pad (char* in, char c, uint32_t amount, bool isLeft)
{
    if (strlen (in) >= amount)
    {
        char* ret = (char*)kmalloc(strlen (in) * sizeof(char));
        memcpy (ret, in, strlen (in));
        return ret;
    }

    uint32_t diff = amount - strlen (in);
    if (isLeft)
    {
        char* ret = (char*)kmalloc((strlen (in) + diff) * sizeof(char));
        for (uint32_t i = 0; i < diff; i++)
            ret [i] = c;
        memcpy (ret + diff, in, strlen (in));
        return ret;
    }
    else
    {
        char* ret = (char*)kmalloc((strlen (in) + diff) * sizeof(char));
        for (uint32_t i = 0; i < diff; i++)
            ret [i + strlen (in)] = c;
        memcpy (ret, in, strlen (in));
        return ret;
    }
}

uint32_t atoio (char* input)
{
    uint32_t c, n;
    n = 0;
    for (c = 0; input [c] != '\0'; c++)
    {
        n = n * 10 + input [c] - '0';
    }
    return n;
}

char* convertToUnit (uint32_t input)
{
    if (input >= 1073741824)
    {
        char* tmp = iotoa ((uint32_t)input / 1073741824);
        char* res = strcat (tmp, " GB");
        kfree(tmp); //Gettin' down and dirty with memory management
        return res;
    }

    else if (input >= 1048576)
    {
        char* tmp = iotoa ((uint32_t)input / 1048576);
        char* res = strcat (tmp, " MB");
        kfree(tmp);
        return res;
    }

    else if (input >= 1024)
    {
        char* tmp = iotoa ((uint32_t)input / 1024);
        char* res = strcat (tmp, " KB");
        kfree(tmp);
        return res;
    }

    else
    {
        char* tmp = iotoa ((uint32_t)input);
        char* res = strcat (tmp, " B");
        kfree(tmp);
        return res;
    }
}

char* strdup(const char* in)
{
    char* ret = kmalloc(sizeof(char) * strlen(in) + 1);
    strcpy(ret, in);
    return ret;
}
