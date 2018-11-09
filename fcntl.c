#include"fcntl.h"

bool S_ISDIR(unsigned short mode)
{
    return mode&0100000;
}

bool S_ISREG(unsigned short mode)
{
    return mode&0040000;
}
