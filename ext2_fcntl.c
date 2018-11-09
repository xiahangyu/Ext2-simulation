#include"fs.h"
#include"ext2_fcntl.h"
#include"blk.h"
#include"namei.h"
#include"task_struct.h"
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//------------------------open_create---------------------------------
int myfopen(char *pathname, unsigned short mode)
{
	int fd;
	fd=sys_open(pathname, !O_CREATE, mode);
	if(fd<0)
		printf("(ext2_fcntl.c\\myfopen)failed to open %s\n",pathname);
	else
		printf("----Sucess: open file %s\n",pathname);
    return fd;
}

int myfcreate(char *pathname, unsigned short mode)
{
	int fd;
	fd=sys_open(pathname, O_CREATE, mode);
	if(fd<0)
		printf("(ext2_fcntl.c\\myfopen)failed to create %s\n",pathname);
	else
		printf("----Sucess: create file %s\n",pathname);
    return fd;
}

int sys_open(char *pathname, unsigned short flag, unsigned short mode)
{
    struct m_inode *mres_inode;
    struct file *f;
    int i,fd;

    //返回文件的内存i节点，不存在则按flag的设置决定是否需要创建
    if( open_namei(pathname, flag, mode, &mres_inode)<0 )
	{	
		//printf("(ext2_fcntl.c\\sys_open)failed to open_namei\n");
        return -ENOENT;
	}
	

	//打开方式需要建立连接,返回fd值
	//创建file_table到m_inode的映射
	f=file_table;
	for(i=0;i<NR_FILE;i++,f++)
	{//找到file_table下表最小的空项用来创建新的file
		if(!f->f_count)
			break;
	}
	if(i==NR_FILE)
	{
		//printf("(ext2_fcntl.c\\sys_open) file_table failed\n");
		return -EINVAL;
	}
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
	{
		//printf("(ext2_fcntl.c\\sys_open) flip failed\n");
		return -EINVAL;
	}
	//在文件描述符和file结构体间建立连接
	current->files->flip[fd]=f;
	f->f_count++;
	printf("open %s: i_number(%d) fd(%d)\n", pathname, mres_inode->i_number, fd);
	
    return fd;
}

//------------------------read--------------------------
int myfread(int fd, char *buf, int count)
{
    struct file * f=current->files->flip[fd];
    struct m_inode * inode;

    if(fd>=NR_OPEN || count<0 || !(f=current->files->flip[fd]) )
        return -EINVAL;

    inode=f->f_inode;
    if(!count || f->f_pos==inode->i_size)
        return 0;

    if(count + f->f_pos > inode->i_size)
        count=inode->i_size-f->f_pos;
    return file_read(inode, f, buf, count);
}

int file_read(struct m_inode * inode, struct file * f, char * buf, int count)
{
    char *buffer=NULL;
    int block,i;
    int left, chars;

    left=count;
    /*if(left<=0)
        return 0;*/

    chars=0;
    while(left)
    {
        buffer=NULL;
        if(block=address_map(inode, f->f_pos/BLOCK_SIZE, 0))
        {
            if(!(buffer=block_read(block)))
                break;
        }

        if(buffer)
        {
            char *p=buf+count-left;
            block=f->f_pos % BLOCK_SIZE;
            chars=min(BLOCK_SIZE - block, left);
            
			for(i=0;i<chars;i++)
				buffer[i]=p[i];

            left-=chars;
            free(buffer);
        }
    }
    return count-left;
}

//-------------------write----------------------------
int myfwrite(int fd, char * buf, int count)
{
	int num=0;
	struct file *f=current->files->flip[fd];
	struct m_inode * inode;

	if(fd>=NR_OPEN || count<0 || !(f=current->files->flip[fd]))
		return -EINVAL;
	if(!count)
		return 0;

	inode=f->f_inode;
	if(S_ISREG(inode->i_mode))
		 return file_write(inode, f, buf, count);
	
	return -EINVAL;
}

int file_write(struct m_inode *inode, struct file *f, char * buf, int count)
{
	char *buffer, *p;
	int block, c;	
	int i=0;	//已经读入的数据长度
	unsigned int pos=f->f_pos;
	
	while(i<count)
	{
		if( !( block=address_map(inode, pos/BLOCK_SIZE, 1) ) )
			break;
		if( !( buffer=block_read(block) ) )
			break;

		c = pos % BLOCK_SIZE ;
		p= c + buffer;
		c= BLOCK_SIZE - c;

		if(c>count-i) c=count-i;

		pos+=c;
		if(pos>(inode->i_size))
		{
			inode->i_size=pos;
			inode->i_dir_acl=1;
		}
		i+=c;
		while( (c--)>0 )
			*(p++)=*(buf++);

		block_write(block,buffer);
		free(buffer);
	}
	
	f->f_pos=pos;
	return (i?i:-1);
}


//------------------lseek--------------------
int myflseek(int fd, unsigned int pos, int mode)
{
	current->files->flip[fd]->f_pos=pos+mode;
	return 0;
}

//-----------------close----------------
void myfclose(int fd)
{
	
}

//---------------Flag--------------------
int S_ISDIR(unsigned short mode)
{
    return mode&0100000;
}

int S_ISREG(unsigned short mode)
{
    return mode&0040000;
}
