#ifndef NAMEI_H_INCLUDED
#define NAMEI_H_INCLUDED

#include"fs.h"
#include"m_inode.h"

 //解析pathname路径获得文件m_inode
int open_namei(char *pathname, unsigned short flag, unsigned short mode, struct m_inode **res_inode);
//为open_namei提供从pathname中解析出name和namelen,以及文件父目录(最后一个目录节点，也叫做枝梢i节点)的m_inode的功能
struct m_inode * dir_namei(char *pathname, int *namelen, const char **name);
//为dir_namei提供从pathname中解析文件父目录(最后一个目录节点，也叫做枝梢i节点)的m_inode的功能
struct m_inode * get_dir(char *pathname);
char * add_entry(struct m_inode * dir, char * name, int namelen, struct dir_entry **res_dir);
char * find_entry(struct m_inode *dir, char * name, int namelen, struct dir_entry **res_dir);  //返回name对应的目录项res_dir和目录项所在的块数据（返回值）

struct m_inode * iget(unsigned int i_number); //获取i节点号为i_number的i节点信息
void iput(struct m_inode *inode);

void read_inode(struct m_inode * inode);

#endif // NAMEI_H_INCLUDED
