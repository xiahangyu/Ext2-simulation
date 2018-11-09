#ifndef M_INODE_H_INCLUDED
#define M_INODE_H_INCLUDED

#include"fs.h"
#include"type.h"
#include"namei.h"

#define Ext2_N_BLOCKS 13
struct m_inode{
    _u16	i_mode; /* �ļ����ͺͷ���Ȩ�� */
    _u16	i_uid;	/* �ļ�ӵ���߱�ʶ��*/
    _u32	i_size; /* ���ֽڼƵ��ļ���С */
    _u32	i_atime; /* �ļ������һ�η���ʱ�� */
    _u32	i_ctime; /* �ýڵ�����޸�ʱ�� */
    _u32	i_mtime; /* �ļ����ݵ�����޸�ʱ�� */
    _u32	i_dtime; /* �ļ�ɾ��ʱ�� */
    _u16	i_gid;	/* �ļ����û����־�� */
    _u16	i_links_count; /* �ļ���Ӳ���Ӽ��� */
    _u32	i_blocks; /* �ļ���ռ������ÿ����512�ֽڼƣ�*/
    _u32	i_flags; /* ���ļ��ķ�ʽ */
    _u32	i_zone[Ext2_N_BLOCKS];  /* ָ�����ݿ��ָ������ */
    _u32	i_version;	  /* �ļ��İ汾�ţ����� NFS�� */
    _u32	i_file_acl; /*�ļ����ʿ��Ʊ��Ѳ���ʹ�ã� */
    _u32	i_dir_acl;	 /*Ŀ¼ ���ʿ��Ʊ��Ѳ���ʹ�ã�*/
    _u8	    l_i_frag;	 /* ÿ���е�Ƭ�� */
    _u32    i_faddr;  /* Ƭ�ĵ�ַ */
    _u16 i_count;
    _u32 i_number;
    _u8  i_flag;
    struct m_inode * i_forw;
    struct m_inode * i_back;
};

struct m_inode *new_inode();
int address_map(struct m_inode *dir,int i_zone_num, int create);

#endif // M_INODE_H_INCLUDED
