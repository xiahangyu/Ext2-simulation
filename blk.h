#ifndef BLK_H_INCLUDED
#define BLK_H_INCLUDED

#include"fs.h"
#include"sb.h"

#define MAX_GOOD_BLOCKS 256

static int free_block_buffer[MAX_GOOD_BLOCKS];

char * block_read(int blk);
void block_write(int blk, char * buffer);

int get_free_block();   //·µ»ØÒ»¸ö¿ÕÏÐ¿é¿éºÅ
void rec_free_block(int blk);

#endif // BLK_H_INCLUDED
