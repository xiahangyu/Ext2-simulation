#ifndef INODE_H_INCLUDED
#define INODE_H_INCLUDED

struct inode{
    _u16 i_mode;
    _u16 i_uid;
    _u32 i_size;
    _u32 i_atime;
    _u32 i_ctime;
    _u32 i_mtime;
    _u16 i_gid;
    _u16 i_links_count;
    _u32 i_blocks;
    _u32 i_flags;
    _u32 i_block[Ext2_N_BLICKS];
}

#endif // INODE_H_INCLUDED
