#ifndef INODE_H_INCLUDED
#define INODE_H_INCLUDED

#include"fs.h"
#include"type.h"

#define INODES_PER_BLOCK (BLOCK_SIZE/128)
#define Ext2_N_BLOCKS 13

struct d_inode{
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
    _u32	i_block[Ext2_N_BLOCKS];  /* ָ�����ݿ��ָ������ */
	_u32	i_version;	  /* �ļ��İ汾�ţ����� NFS�� */
	_u32	i_file_acl; /*�ļ����ʿ��Ʊ��Ѳ���ʹ�ã� */
	_u32	i_dir_acl;	 /*Ŀ¼ ���ʿ��Ʊ��Ѳ���ʹ�ã�*/
	_u8	l_i_frag;	 /* ÿ���е�Ƭ�� */
	_u32   i_faddr;  /* Ƭ�ĵ�ַ */
};

#endif // INODE_H_INCLUDED
