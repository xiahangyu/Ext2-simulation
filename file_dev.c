#include"fs.h"
#include"file_dev.h"
#include"blk.h"
#include"sb.h"
#include"namei.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct su_block * get_super()
{
    char * buffer;
    int block;
    struct su_block * sb;

    block=1;
    if( !( buffer=block_read(block) ) )
        return NULL;
    sb=(struct su_block *)malloc( sizeof(super_block_buffer) );
    *(sb)=*( (struct su_block *)buffer );
    free(buffer);
    return sb;
}

void read_inode(struct m_inode *inode)
{//从硬盘上读出inode->i_number的外存i节点，将其内容赋给inode
    char * buffer;
    int block;
    struct su_block *sb;

    if( !( sb=get_super() ) )
    {
        free(sb);
        return ;
    }

    block=2 + sb->s_imap_blocks + sb->s_zmap_blocks + inode->i_number/INODES_PER_BLOCK ;
    free(sb);

    if(!(buffer=block_read(block)))
        return ;

    *(struct d_inode *)inode=((struct d_inode *)buffer)[inode->i_number%INODES_PER_BLOCK];
    free(buffer);
}


int get_free_inode()
{
	return k--;
	/*
    int num;
    num= super_block_buffer.s_inode[super_block_buffer.s_ninode-1];
    if(super_block_buffer.s_ninode==1)
    {
        if(super_block_buffer.s_inode[super_block_buffer.s_ninode-1]+100>INODES)
        {
            printf("no enough free inode space.");
            return -1;
        }
        else
            free_inode_init(super_block_buffer.s_inode[super_block_buffer.s_ninode-1]);
    }
    return num;
	*/
}

k=100;
void rec_free_inode(unsigned int num)
{
	if(num==k) k++;
	/*
    if(super_block_buffer.s_ninode==100)
    {
        if(num<super_block_buffer.s_inode[0])
            super_block_buffer.s_inode[0]=num;
        else
            return;
    }
    else if(num<super_block_buffer.s_inode[0])
        super_block_buffer.s_inode[0]=num;
    else
    {
        super_block_buffer.s_inode[super_block_buffer.s_ninode]=num;
        super_block_buffer.s_ninode++;
    }
	*/
}

