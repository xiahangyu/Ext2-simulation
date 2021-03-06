#ifndef SB_H_INCLUDED
#define SB_H_INCLUDED

#include"type.h"

struct su_block{
        _u32    s_inodes_count; //文件系统中索引节点总数
        _u32    s_block_count;//文件系统中总块数
        _u32	s_r_blocks_count;	  /* 为超级用户保留的块数 */
        _u32    s_imap_blocks;
        _u32    s_zmap_blocks;
        _u32	s_free_blocks_count;	/*文件系统中空闲块总数 */
        _u32	s_free_inodes_count;	/*文件系统中空闲索引节点总数*/
        _u32	s_first_data_block;	/* 文件系统中第一个数据块 */
        _u32	s_log_block_size;	 /* 用于计算逻辑块大小 */
        _u32	s_log_frag_size;	 /* 用于计算片大小 */
        _u32	s_blocks_per_group;	/* 每组中块数 */
        _u32	s_frags_per_group;  /* 每组中片数 */
        _u32	s_inodes_per_group;	/* 每组中索引节点数 */
        _u32	s_mtime;   /*最后一次安装操作的时间 */
        _u32	s_wtime;  /*最后一次对该超级块进行写操作的时间 */
        _u16	s_mnt_count;	/* 安装计数 */
        _u16	s_max_mnt_count;	 /* 最大可安装计数 */
        _u16	s_magic;  /* 用于确定文件系统版本的标志 */
        _u16	s_state;  /* 文件系统的状态*/
        _u16	s_errors;  /* 当检测到有错误时如何处理 */
        _u16	s_minor_rev_level; 	/* 次版本号 */
        _u32	s_lastcheck;	/* 最后一次检测文件系统状态的时间 */
        _u32	s_checkinterval; /* 两次对文件系统状态进行检测的间隔时间 */
        _u32	s_rev_level;	/* 版本号 */
        _u16	s_def_resuid;	 /* 保留块的默认用户标识号 */
        _u16	s_def_resgid;  /* 保留块的默认用户组标识号*/
        _u32    s_first_ino;  /* 第一个非保留的索引节点 */
        _u16    s_inode_size;  /* 索引节点的大小 */
        _u16    s_block_group_nr;  /* 该超级块的块组号 */
        _u32    s_feature_compat;  /* 兼容特点的位图*/
        _u32    s_feature_incompat;  /* 非兼容特点的位图 */
        _u32    s_feature_ro_compat;  /* 只读兼容特点的位图*/
        _u8     s_uuid[16];  /* 128位的文件系统标识号*/
        _u8     s_volume_name[16];  /* 卷名 */
        _u8     s_last_mounted[64];  /* 最后一个安装点的路径名 */
        _u32    s_algorithm_usage_bitmap;  /* 用于压缩*/
        _u8     s_prealloc_blocks;  /* 预分配的块数*/
        _u8     s_prealloc_dir_blocks;  /* 给目录预分配的块数 */
        _u16    s_padding1;  /* 填充 */
        _u32    s_free[256];
        _u32    s_nfree;
        _u32    s_inode[100];
        _u32    s_ninode;
        _u32    s_reserved[204];  /* 用null填充块的末尾 */
};

#endif // SB_H_INCLUDED

