#ifndef EXT2_FCNTL_H_INCLUDED
#define EXT2_FCNTL_H_INCLUDED

#include"fs.h"
#include"namei.h"

#define O_ACCMODE 00003

#define O_RDONLY 00 //只读方式打开文件
#define O_WRONLY 01 //只写方式打开文件
#define O_RDWR   02 //读写方式打开文件

#define O_CREATE 00100

#define I_REGULAR 0040000
#define I_DIR 0100000

int myfopen(char *pathname, unsigned short mode);
int sys_open(char *pathname, unsigned short flag, unsigned short mode);

int myfcreate(char *pathname, unsigned short mode);

int myfread(int fd, char *buffer, int count);
int file_read(struct m_inode * inode, struct file * f, char * buf, int count);

int myfwrite(int fd, char * buffer, int count);
int file_write(struct m_inode *inode, struct file *f, char * buf, int count);

int myflseek(int fd, unsigned int pos, int mode);

void myfclose(int fd);
//-------------------------------
int S_ISDIR(unsigned short mode);
int S_ISREG(unsigned short mode);

#endif // FCNTL_H_INCLUDED
