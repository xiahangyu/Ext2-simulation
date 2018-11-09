#ifndef FCNTL_H_INCLUDED
#define FCNTL_H_INCLUDED

#include"fs.h"

#define O_RDONLY 00 //ֻ����ʽ���ļ�
#define O_WRONLY 01 //ֻд��ʽ���ļ�
#define O_RDWR   02 //��д��ʽ���ļ�
#define O_CREATE 00100
#define O_ACCMODE 00003

int myfopen(const char *pathname, const char *mode=O_RDWR);
int myfcreate(const char *pathname, const char *mode=O_RDWR);
int myread(int fd, char *buffer, int len);

bool S_ISDIR(unsigned short mode);
bool S_ISREG(unsigned short mode);

#endif // FCNTL_H_INCLUDED
