#ifndef FILE_DEV_H_INCLUDED
#define FILE_DEV_H_INCLUDED

#include"fs.h"
#include"sb.h"

struct su_block * get_super();
void read_inode(struct m_inode * inode);
void write_inode(struct m_inode * inode);

int get_free_inode();
void rec_free_inode(unsigned int num);

unsigned int k;

#endif // FILE_DEV_H_INCLUDED
