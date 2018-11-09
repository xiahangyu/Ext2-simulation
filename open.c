#include"fs.h"
#include"ext2_fcntl.h"
#include"namei.h"

int myfopen(const char *pathname, unsigned short mode=O_RDWR)
{
    struct m_inode *mres_inode,*m_temp;
    file *f;
    int i,fd;

    //����ļ������ڣ�����0
    if(open_namei(pathname, O_CREATE, mode, &mres_inode)<0)
        return -ENOENT;


    //����file_table��m_inode��ӳ��
    f=file_table;
    for(i=0;i<NR_FILE;i++,f++)
    {//�ҵ�file_table�±���С�Ŀ������������µ�file
        if(!f->count)
            break;
    }
    if(i==NR_FILE)
        return -EINVAL;
    //�ڿ�file_table������Ӵ��ļ�file�ṹ��Ϣ
    f->f_pos=0;
        //f->f_mode=mode;
        //f->f_flags=0;
    f->f_count=0;
        //struct file *f_next, *f_prev;
    f->f_inode=mres_inode;
    mres_inode->i_count++;


    //�����ļ�������
    for(fd=0;fd<NR_OPEN;fd++)
    {//�ҵ��±���С�Ŀ�fd
        if(!current->files->flip[fd])
            break;
    }
    if(fd==NR_OPEN)
        return -EINVAL;
    //���ļ���������file�ṹ��佨������
    current->files->flip[fd]=f;
    f->f_count++;

    return fd;
}
