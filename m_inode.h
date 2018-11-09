#ifndef M_INODE_H_INCLUDED
#define M_INODE_H_INCLUDED

#include"fs.h"
#include"type.h"
#include"namei.h"

#define Ext2_N_BLOCKS 13
struct m_inode{
    _u16	i_mode; /* 文件类型和访问权限 */
    _u16	i_uid;	/* 文件拥有者标识号*/
    _u32	i_size; /* 以字节计的文件大小 */
    _u32	i_atime; /* 文件的最后一次访问时间 */
    _u32	i_ctime; /* 该节点最后被修改时间 */
    _u32	i_mtime; /* 文件内容的最后修改时间 */
    _u32	i_dtime; /* 文件删除时间 */
    _u16	i_gid;	/* 文件的用户组标志符 */
    _u16	i_links_count; /* 文件的硬链接计数 */
    _u32	i_blocks; /* 文件所占块数（每块以512字节计）*/
    _u32	i_flags; /* 打开文件的方式 */
    _u32	i_zone[Ext2_N_BLOCKS];  /* 指向数据块的指针数组 */
    _u32	i_version;	  /* 文件的版本号（用于 NFS） */
    _u32	i_file_acl; /*文件访问控制表（已不再使用） */
    _u32	i_dir_acl;	 /*目录 访问控制表（已不再使用）*/
    _u8	    l_i_frag;	 /* 每块中的片数 */
    _u32    i_faddr;  /* 片的地址 */
    _u16 i_count;
    _u32 i_number;
    _u8  i_flag;
    struct m_inode * i_forw;
    struct m_inode * i_back;
};

struct m_inode *new_inode();
int address_map(struct m_inode *dir,int i_zone_num, int create);

#endif // M_INODE_H_INCLUDED
