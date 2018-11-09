#include"fs.h"
#include"ext2_fcntl.h"
#include"namei.h"

int myfopen(const char *pathname, unsigned short mode=O_RDWR)
{
    struct m_inode *mres_inode,*m_temp;
    file *f;
    int i,fd;

    //如果文件不存在，返回0
    if(open_namei(pathname, O_CREATE, mode, &mres_inode)<0)
        return -ENOENT;


    //创建file_table到m_inode的映射
    f=file_table;
    for(i=0;i<NR_FILE;i++,f++)
    {//找到file_table下表最小的空项用来创建新的file
        if(!f->count)
            break;
    }
    if(i==NR_FILE)
        return -EINVAL;
    //在空file_table项中添加打开文件file结构信息
    f->f_pos=0;
        //f->f_mode=mode;
        //f->f_flags=0;
    f->f_count=0;
        //struct file *f_next, *f_prev;
    f->f_inode=mres_inode;
    mres_inode->i_count++;


    //创建文件描述字
    for(fd=0;fd<NR_OPEN;fd++)
    {//找到下表最小的空fd
        if(!current->files->flip[fd])
            break;
    }
    if(fd==NR_OPEN)
        return -EINVAL;
    //在文件描述符和file结构体间建立连接
    current->files->flip[fd]=f;
    f->f_count++;

    return fd;
}
