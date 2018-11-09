#include"fs.h"
#include"ext2_fcntl.h"
#include"blk.h"
#include"string.h"

int myfread(int fd, char *buf, int count)
{
    struct file * f;
    struct m_inode * inode;

    if(fd>=NR_OPEN || count<0 || !(f=current->files->flip[fd]) )
        return -EINVAL;

    if(!count || f->f_pos==inode->i_size)
        return 0;

    if(count + f->f_pos > inode->i_size)
        count=inode->i_size-f->f_pos;
    return
        file_read(inode, f, buf, count);
}
