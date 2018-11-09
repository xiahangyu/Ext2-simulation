#ifndef NAMEI_H_INCLUDED
#define NAMEI_H_INCLUDED

#include"fs.h"
#include"m_inode.h"

 //����pathname·������ļ�m_inode
int open_namei(char *pathname, unsigned short flag, unsigned short mode, struct m_inode **res_inode);
//Ϊopen_namei�ṩ��pathname�н�����name��namelen,�Լ��ļ���Ŀ¼(���һ��Ŀ¼�ڵ㣬Ҳ����֦��i�ڵ�)��m_inode�Ĺ���
struct m_inode * dir_namei(char *pathname, int *namelen, const char **name);
//Ϊdir_namei�ṩ��pathname�н����ļ���Ŀ¼(���һ��Ŀ¼�ڵ㣬Ҳ����֦��i�ڵ�)��m_inode�Ĺ���
struct m_inode * get_dir(char *pathname);
char * add_entry(struct m_inode * dir, char * name, int namelen, struct dir_entry **res_dir);
char * find_entry(struct m_inode *dir, char * name, int namelen, struct dir_entry **res_dir);  //����name��Ӧ��Ŀ¼��res_dir��Ŀ¼�����ڵĿ����ݣ�����ֵ��

struct m_inode * iget(unsigned int i_number); //��ȡi�ڵ��Ϊi_number��i�ڵ���Ϣ
void iput(struct m_inode *inode);

void read_inode(struct m_inode * inode);

#endif // NAMEI_H_INCLUDED
