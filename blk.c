#include"fs.h"
#include"blk.h"
#include"sb.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * block_read(int blk)
{
    char * buffer=(char *)malloc(BLOCK_SIZE);
    if( fseek(fd, blk*BLOCK_SIZE, SEEK_SET) )
        printf("(blk.c)seek failed in read_block.\n");

    if( !fread(buffer, 1, BLOCK_SIZE, fd) )
    {
        printf("(blk.c)read failed in read_block.\n");
        return NULL;
    }
    return buffer;
}

void block_write(int blk, char * buffer)
{
    if (fseek(fd, blk*BLOCK_SIZE, SEEK_SET))
		printf("(blk.c)seek failed in write_block.\n");

    if (BLOCK_SIZE!= fwrite(buffer, 1, BLOCK_SIZE,fd))
		printf("(blk.c)write failed in write_block.\n");
}


int get_free_block()
{
    int blk,i;

    blk = super_block_buffer.s_free[super_block_buffer.s_nfree-1];

    if (super_block_buffer.s_nfree==1)
    {
        if(fseek(fd, BLOCK_SIZE*blk, SEEK_SET))
			printf("(blk.c)seek failed in get_free_block.\n");

        if(MAX_GOOD_BLOCKS*sizeof(int)!=fread(&free_block_buffer, 1, MAX_GOOD_BLOCKS*sizeof(int), fd))
			printf("(blk.c)read failed in get_free_block.\n");

        for(i=0;i<MAX_GOOD_BLOCKS;i++)
        {
            super_block_buffer.s_free[i]=free_block_buffer[i];
        }
        super_block_buffer.s_nfree=MAX_GOOD_BLOCKS;
    }
    else super_block_buffer.s_nfree--;

    return blk;
}

void rec_free_block(int blk)
{
    int i;

    if (super_block_buffer.s_nfree == MAX_GOOD_BLOCKS)
        {
            for(i=0;i<MAX_GOOD_BLOCKS;i++)
            {
                free_block_buffer[i]=super_block_buffer.s_free[i];
            }
            if(fseek(fd, BLOCK_SIZE*blk, SEEK_SET))
				printf("seek failed in rec_free_block.\n");

            if(MAX_GOOD_BLOCKS*sizeof(int)!=fwrite(&free_block_buffer, 1, MAX_GOOD_BLOCKS*sizeof(int), fd ) )
				printf("write failed in rec_free_block.\n");

            super_block_buffer.s_nfree=1;
            super_block_buffer.s_free[super_block_buffer.s_nfree-1]=blk;

        }
    else
    {
        super_block_buffer.s_nfree++;
        super_block_buffer.s_free[super_block_buffer.s_nfree]=blk;
    }
}
