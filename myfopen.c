
#include"fs.h"
#include"myfopen.h"

int myfopen(const char *dir, const char *name, const char *mode)
{
    int i;
    for(i=0;i<NR_OPEN;i++)
    {
        if(!current->files->fd[i])
            continue;
    }
    if(i==NR_OPEN)
        return -1;

    for(i=0;i<NR_FILE;i++)
    {
        if(!sys_file_table[i])
            continue;
    }
    if(i==NR_FILE)
        return -1;

    for(i=0;i<NR_INODE;i++)
    {
        if(!sys_inode[i])
            continue;
    }
    if(i==NR_INODE)
        return -1;


}
