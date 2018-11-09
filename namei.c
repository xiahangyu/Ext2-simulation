#include"fs.h"
#include"ext2_fcntl.h"
#include"namei.h"
#include"blk.h"
#include"sb.h"
#include"file_dev.h"
#include<stdio.h>
#include<stdlib.h>

int open_namei(char *pathname, unsigned short flag, unsigned short mode, struct m_inode **res_inode)
{
    char * basename, *buffer;
    int namelen,i_number,block;
    struct m_inode *dir,*inode;
    struct dir_entry *de;

    mode&=0777 & ~UMASK;
    mode|=I_REGULAR;
    if(!(dir=dir_namei(pathname,&namelen,&basename)))
    {
		printf("(namei.c\\open_namei)dir_namei failed\n");
        return -ENOENT;
    }

    //if(!namelen){//处理打开文件为目录的情况}

    buffer=find_entry(dir, basename, namelen, &de);
    if(!buffer)
    {//文件不存在时
        if(!(flag & O_CREATE))
        {
            iput(dir);
            return -EINVAL;
        }
        else    
        {
            if( !( inode=new_inode() ) )
            {
                iput(dir);
                return -ENOSPC;
            }
			else
			{
				if(!(buffer=add_entry(dir, basename, namelen, &de)))
				{
					printf("(namei.c\\open_namei)add_entry failed\n");
					return -ENOSPC;
				}
				dir->i_dir_acl=1;
				inode->i_links_count++;

				block=de->i_number;
				de->i_number=inode->i_number;

				block_write(block,buffer);
				free(buffer);
			}
        }
    }
    else
    {
        i_number=de->i_number;
        inode=iget(i_number);   //iget的下一层是inode_read
                                //将i节点号为i_number的外存节点调入内存或者返回已存在在内存与之相对应的内存i节点地址
        inode->i_number=i_number;
        inode->i_dir_acl=1;
		free(buffer);
    }

	if(dir->i_number!=1)
		iput(dir);
	
    *res_inode=inode;
    return 1;
}

struct m_inode * dir_namei(char *pathname, int *namelen, const char **name)
{//找到pathname路径文件所在目录的m_inode,和文件名
    char c;
    char *basename,*pn;
    struct m_inode *dir;

    pn=basename=pathname;
    while(c=*pathname)
    {
        if(c=='/')
            basename=pathname;
        pathname++;
    }
	if(pn==basename)
		*namelen=pathname-basename;
    else
		*namelen=pathname-basename-1;
    *name=basename;

    if(!(dir=get_dir(pn)))
	{
		printf("(namei.c\\dir_namei)get_dir failed\n");
        return NULL;
	}
    return dir;
}

struct m_inode * get_dir(char *pathname)
{//根据pathname找到文件所在目录的内存i节点，不存在则从外存调入
    char c;
    char *thisname,*buffer;
    int thislen;
    struct m_inode *inode;
    struct dir_entry *de;
	
    if(!current->fs->root && !current->fs->root->i_count)
	{
		printf("(namei.c\\get_dir)current->fs->root failed\n");
        return NULL;
	}
    if(!current->fs->pwd && !current->fs->pwd->i_count)
	{
		printf("(namei.c\\get_dir)current->fs->pwd failed\n");
        return NULL;
	}

    if(pathname[0]=='/')
    {//根目录绝对索引
        inode=current->fs->root;
        pathname++;
    }
    else if(pathname[0])
    {//当前目录相对索引
        inode=current->fs->pwd;
    }
    else
	{
		printf("(namei.c\\get_dir)pathname failed\n");
        return NULL;
	}
    //inode->i_count++; //当inode为m_inode的时候需要下面的注释的代码
    //if(!S_ISDIR(inode->i_mode))  {iput(inode); return NULL;}  //如果dir不是目录

    while(1)
    {
        thisname=pathname;
        for(thislen=0; c=(char)*(pathname++) && c!='/'; thislen++)
            ;
        if(!c)  //路径指向当前目录下的文件,直接返回当前目录
            return inode;

        if(buffer)
        {
            free(buffer);
            buffer=NULL;
        }
        if(!(buffer=find_entry(inode,thisname,thislen,&de)))
        {
			printf("(namei.c\\get_dir)find_entry failed\n");
            iput(inode);
            return NULL;
        }

        if(!(inode=iget(de->i_number)))
		{
			printf("(namei.c\\get_dir)iget failed\n");
            return NULL;
		}
    }
}

char * add_entry(struct m_inode * dir, char * name, int namelen, struct dir_entry **res_dir)
{//在目录dir数据区里添加目录项
    int i;
    unsigned int block,i_zone_number;
    char * buffer;
    struct dir_entry *de;

    *res_dir=NULL;
    if(!namelen)
        return NULL;

    i_zone_number=0;
    if( !( block=address_map(dir, i_zone_number++, 1) ) )
	{
		printf("(namei.c\\add_entry)address_map failed\n");
        return NULL;
	}
    if( !( buffer=block_read( block )) )
	{
		printf("(namei.c\\add_entry)block_read failed\n");
        return NULL;
	}

    i=0;
    de=(struct dir_entry *)buffer;
    while(1)
    {//从逻辑块0块开始遍历，查找空的目录项
        if( (char *)de >= buffer + BLOCK_SIZE )
        {
            free(buffer);
            buffer=NULL;
            if( !( block=address_map( dir, i_zone_number, 1) ) )
            {//查找表的下一快，如果没有则申请一块
				printf("(namei.c\\add_entry)while-address_map failed\n");
                return NULL;
            }
            if( !( buffer=block_read( block ) ))
			{
				printf("(namei.c\\add_entry)while-block_read failed\n");
                return NULL;
			}

            de=(struct dir_entry *)buffer;
            i_zone_number++;
        }

        if( i*sizeof(struct dir_entry)>= dir->i_size )
        {//表中没有空闲项，则在表的最后添加一个空闲项
            de->i_number=0;
            dir->i_size+=sizeof(struct dir_entry);
        }

        if(!de->i_number)
        {
            for(i=0;i<namelen;i++)
                de->name[i]=name[i];
            de->namelen=namelen;

			de->i_number=block;	//用于返回修改块号

            if(i_zone_number>dir->i_blocks)
                dir->i_blocks=i_zone_number;
            dir->i_dir_acl=1;
            *res_dir=de;

            return buffer;
        }
        de++;
        i++;
    }

    free(buffer);
	printf("(namei.c\\add_entry)return failed\n");
    return NULL;
}


int match(char * name, int namelen, struct dir_entry *de)
{
	int i;
    if( namelen != (de->namelen) )
        return -1;
    for(i=0;i<namelen;i++)
	{
        if(name[i]!=de->name[i])
            return -1;
	}

    return 0;
}
char * find_entry(struct m_inode *dir, char *name, int namelen, struct dir_entry **res_dir)
{//在目录中检索name名的inode号，并返回目录项
    int entries;
    int block,i;
    char * buffer;
    struct dir_entry * de;

    //int remainder,answer;
    //remainder=dir->i_size%sizeof(struct dir_entry);
    //answer=dir->i_size/sizeof(struct dir_entry);
    //entries= (remainder==0) ? answer : answer+1;
	entries=dir->i_size/sizeof(struct dir_entry);

    if(!namelen)
        return NULL;

    i=0;
    if( !( block=address_map( dir, i, 0) ) )
        return NULL;
    if(!(buffer=block_read(block)))
        return NULL;
	
    de=(struct dir_entry *)buffer;
    while(i<=entries)
    {
        if( (char *)de >= BLOCK_SIZE + buffer)
        {
            free(buffer);
            buffer=NULL;
            if( !(block=address_map( dir, i/DIR_ENTRIES_PER_BLOCK, 0)) || !(buffer=block_read(block)))
            {
                if(buffer)
                    free(buffer);
                break;
            }
            de=(struct dir_entry *)buffer;
        }
        if( match(name,namelen,de)==0 )
        {
			//printf("match %s %d\n",de->name,de->i_number);
            *res_dir=de;
            return buffer;
        }
        de++;
        i++;
    }
    free(buffer);
    return NULL;
}

struct m_inode * iget(unsigned int i_number)
{//返回i节点号为i_number的内存i节点，不存在则将外存的调入
    struct m_inode *m_temp;

    for( m_temp=inode_table.i_back ; m_temp!=NULL; m_temp=m_temp->i_back)
    {
        if( m_temp->i_number == i_number)
        {
            return m_temp;
        }
    }

    if(!m_temp)
    {
        m_temp=(struct m_inode *)malloc( sizeof(struct m_inode) );
        m_temp->i_number=i_number;
        m_temp->i_size=0;
		m_temp->i_count=0;
		m_temp->i_forw=NULL;
		m_temp->i_back=NULL;

        read_inode(m_temp); //read_inode是设备操作
    }

    m_temp->i_back=inode_table.i_back;
    m_temp->i_forw=&inode_table;
    inode_table.i_back=m_temp;

    return m_temp;
}

void iput(struct m_inode * inode)
{//将内存i节点写回磁盘
	char * buffer;
	int block,pos;
	struct su_block *sb;

	if((inode->i_number)!=1)
	{
		if(!( --(inode->i_count) ))
			return ;
		if( !inode->i_dir_acl )
			return ;
	}
	
    if( ! (sb=get_super()) )
    {
        free(sb);
        return ;
    }

	inode->i_dir_acl=0;
	block=2 + sb->s_imap_blocks + sb->s_zmap_blocks + inode->i_number/INODES_PER_BLOCK ;
	if( ! (buffer=block_read(block)) )
	{
		printf("(namei.c)failed to iput\n");
		return ;
	}
	
	pos=( inode->i_number%INODES_PER_BLOCK ) * INODE_SIZE;
	*(struct d_inode *)(buffer+pos)=*( (struct d_inode *)inode );
	block_write(block, buffer);

	if(inode->i_number!=1)
	{
		inode->i_forw->i_back=inode->i_back;
		inode->i_back->i_forw=inode->i_forw;
		free(inode);
	}

	free(buffer);
}
