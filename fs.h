#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED

#include"d_inode.h"
#include"m_inode.h"
#include"task_struct.h"
#include"sb.h"
#include"errno.h"
#include"type.h"
#include<stdio.h>

#define UMASK 0111
#define NR_OPEN 20 //���̿��Դ��ļ��������
#define NR_FILE 64 //����ϵͳ���Դ��ļ��������

#define BLOCKS (1024*256)
#define INODES (BLOCKS/4)
#define Ext2_N_BLOCKS 13
#define BLOCK_SIZE 4096

#define SUPER_BLOCKS 1
#define INODES_PER_BLOCK (BLOCK_SIZE/128)
#define IMAPS INODES/(8*BLOCK_SIZE)
#define ZMAPS BLOCKS/(8*BLOCK_SIZE)
#define INODE_SIZE 128
#define INODE_BLOCKS (INODES*INODE_SIZE/BLOCK_SIZE/2)
#define NORM_FIRSTZONE (1+SUPER_BLOCKS+IMAPS+ZMAPS+INODE_BLOCKS)
#define ROOT_INO 1
#define MAX_GOOD_BLOCKS 256
#define TEST_BUFFER_BLOCKS 16
#define DENTRY_SIZE 256
#define DIR_ENTRIES_PER_BLOCK (BLOCK_SIZE/DENTRY_SIZE)
#define setbit(x,y) x|=(0x01<<y)
#define clrbit(x,y) x&=~(0x01<<y)
#define mark_zone(x) (setbit(zone_map[x],(x)))
#define unmark_zone(x) (clrbit(zone_map[x],(x)))
#define mark_inode(x) (setbit(inode_map[x],(x)))
#define unmark_inode(x) (clrbit(inode_map[x],(x)))

int ZONESIZE ;
int ZONES ;
int FIRSTZONE;

static char zone_map[ZMAPS*BLOCK_SIZE];
static char inode_map[IMAPS*BLOCK_SIZE];
static int free_block_buffer[MAX_GOOD_BLOCKS];
struct d_inode inode_buffer[INODES];
struct su_block super_block_buffer;

FILE *fd;

//--------------------------
struct fs_struct{
    int count;
    struct m_inode *root,*pwd;
    unsigned short umask;
};

struct files_struct{
    int count;
    struct file *flip[NR_OPEN];
    //fd_set close_on_exec;
    //fd_set open_fds;
};

struct file{
    unsigned short f_mode;
    unsigned int f_pos;
    unsigned short f_flags;
    unsigned short f_count;
    //struct file *f_next, *f_prev;
    struct m_inode *f_inode;
    //struct file_operation *f_op;
};

struct dir_entry{
    int i_number;
    char name[250];
    //unsigned short rec_len; //Ŀ¼���
    unsigned char namelen;
    unsigned char file_type;
};

struct file file_table[NR_FILE];
struct m_inode inode_table;

void init_filesystem();     //��ʼ���ļ�ϵͳ
void setup_tables();        //����λͼ
void file_table_init();
void inode_table_init();
void free_inode_init(int setpos);   //����inode��
void free_table_init();             //���п��
void super_block_init();            //������
void make_root_inode();             //���ڵ�
void write_tables();                //���̸�ʽ��
void check_blocks();

#endif // FS_H_INCLUDED
