#include"fs.h"
#include"blk.h"
#include"sb.h"
#include"namei.h"
#include"file_dev.h"
#include<stdio.h>
#include<stdlib.h>

struct m_inode * new_inode()
{//��һ���յ����i�ڵ�����ڴ棬��������Ӧ���ڴ�i�ڵ�
    int i_number,i;
    struct m_inode * inode;

    i_number=get_free_inode();
    inode=iget(i_number);   //��ָ���ڵ�ŵ����i�ڵ�����ڴ�

    inode->i_links_count=0;
    inode->i_size=0;
    inode->i_dir_acl=1;
    inode->i_blocks=0;

    for(i=0;i<Ext2_N_BLOCKS;i++)
        inode->i_zone[i]=0;

    inode->i_mode=~UMASK;

    inode->i_number=i_number;
    inode->i_count=0;
	inode->i_forw=NULL;
	inode->i_back=NULL;

    return inode;
}

int address_map(struct m_inode *inode,int i_zone_num, int create)
{//ͨ��i�ڵ��߼�����Ҷ�Ӧ�������ţ�createΪ1ʱ��Ӧ������Ϊ��ʱ����һ�����п�
    char * buffer;
	int i=0;

	if(i_zone_num<0)
	{
		printf("(m_inode.c\\address_map) <0 \n");
		return 0; 
	}
    else if(i_zone_num<10)
    {
		if(create && !inode->i_zone[i_zone_num])
		{
			if( inode->i_zone[i_zone_num]=get_free_block() )
				inode->i_dir_acl=1;
		}
		return inode->i_zone[i_zone_num];
    }
    else if (i_zone_num>=10 && i_zone_num<10+1024)
    {
		i_zone_num-=10;
		if(create && !inode->i_zone[10])
		{
			if(inode->i_zone[10]=get_free_block())
				inode->i_dir_acl=1;
		}

		if(!( inode->i_zone[10]) )
		{
			printf("(m_inode.c\\address_map) 10~10+1024\n");
			return 0;
		}
		if( !( buffer=block_read( inode->i_zone[10]) ) )
		{
			printf("(m_inode.c\\address_map) 10~10+1024 block_read\n");
			return 0;
		}
	
		i= ( (unsigned int *)buffer )[i_zone_num];
		if( create && !i )
		{
			if(i=get_free_block())
			{
				((unsigned int *)buffer)[i_zone_num]=i;
				block_write(i, buffer);
			}
		}
		
		free(buffer);
    }
    else if (i_zone_num>= 10 + 1024 && i_zone_num < 10 + 1024 + 1024*1024)
    {
		i_zone_num -= 10 +1024;
		if(create && !inode->i_zone[11])
		{
			if(inode->i_zone[11]=get_free_block())
				inode->i_dir_acl=1;
		}

		if(!inode->i_zone[11])
		{
			printf("(m_inode.c\\address_map) 10+1024~10+1024+1024*1024\n");
			return 0;
		}
		if( !( buffer=block_read( inode->i_zone[11] ) ) )
		{
			printf("(m_inode.c\\address_map) 10+1024~10+1024+1024*1024 block_read\n");
			return 0;
		}
	
		i= ( (unsigned int *)buffer )[i_zone_num/1024];
		if(create && !i )
		{
			if(i=get_free_block())
			{
				((unsigned int *)buffer)[i_zone_num/1024]=i;
					block_write(i, buffer);
			}
		}
		free(buffer);

		if(!i)
		{
			printf("(m_inode.c\\address_map) 10+1024~10+1024+1024*1024 ��������\n");
			return 0;
		}
		if( !(buffer=block_read(i) ) )
		{
			printf("(m_inode.c\\address_map) 10+1024~10+1024+1024*1024 �������� block_read\n");
			return 0;
		}
		i=( (unsigned int *)buffer )[i_zone_num%1024];
		if(create && !i)
		{
			if( i=get_free_block() )
			{
				((unsigned int *)buffer)[i_zone_num%1024]=i;
				block_write(i, buffer);
			}
		}

		free(buffer);
    }
	return i;
}

