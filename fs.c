#include"fs.h"
#include"blk.h"
#include"ext2_fcntl.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void setup_tables()
{
   ZONESIZE = 0;
   ZONES = BLOCKS;
   FIRSTZONE = NORM_FIRSTZONE;
	/*
   int i;
   memset(inode_map,0xff,sizeof(inode_map));
   memset(zone_map,0xff,sizeof(zone_map));
   for (i = FIRSTZONE ; i<ZONES ; i++)
   {
       unmark_zone(i);
   }
   for (i = ROOT_INO ; i<INODES ; i++)
   {
       unmark_inode(i);
   }
   //printf("function setup_tables has been done.\n");
   */
}

void file_table_init()
{
	int i;
	for(i=0;i<NR_FILE;i++)
	{
		file_table[i].f_count=0;
		file_table[i].f_inode=NULL;
		file_table[i].f_pos=0;
	}
	printf("file_table has been initialized.\n");
}

void inode_table_init()
{
	inode_table.i_back=NULL;
	printf("inode_table has been initialized.\n");
}

void free_inode_init(int setpos)
{
    int i;
    super_block_buffer.s_ninode=100;
    for(i=0;i<100;i++)
    {
        super_block_buffer.s_inode[i]=setpos+100-i;
    }
    //printf("free inode table has been already filled.\n");
}

void free_table_init()
{
    int blk,i;
    int last=0;
    for (blk=ZONES-1;blk >= FIRSTZONE ;blk--)
    {

        if(blk-MAX_GOOD_BLOCKS>=FIRSTZONE)
        {
            free_block_buffer[0]=last;
            for(i=1;i<MAX_GOOD_BLOCKS;i++)
            {
                free_block_buffer[i]=blk;
                blk--;
            }
            if( fseek(fd, BLOCK_SIZE*blk, SEEK_SET) )
				printf("seek failed in free_table_init.\n");

            if(MAX_GOOD_BLOCKS*sizeof(int)!=fwrite(&free_block_buffer, 1, MAX_GOOD_BLOCKS*sizeof(int), fd))
				printf("write failed in free_table_init.\n");

            last=blk;
        }
        else
        {
			super_block_buffer.s_nfree=0;
            super_block_buffer.s_free[super_block_buffer.s_nfree] = last;
            super_block_buffer.s_nfree++;
            while(blk >= FIRSTZONE)
            {
               super_block_buffer.s_free[super_block_buffer.s_nfree] = blk;
               super_block_buffer.s_nfree++;
               blk--;
            }
        }
    }
    //printf("free block table has been already inited.\n");
}

void super_block_init()
{
    super_block_buffer.s_block_count=BLOCKS;
    super_block_buffer.s_inodes_count=INODES;
    super_block_buffer.s_first_data_block=NORM_FIRSTZONE;
    super_block_buffer.s_log_block_size=BLOCK_SIZE;
    super_block_buffer.s_inode_size=INODE_SIZE;
    super_block_buffer.s_nfree=0;
    super_block_buffer.s_ninode=0;
    free_inode_init(ROOT_INO);
    free_table_init();
    super_block_buffer.s_wtime = time(NULL);
    //printf("super block  has been already inited.\n");

}

void write_tables()
{
    if ( fseek(fd, BLOCK_SIZE, SEEK_SET) )
        printf("(fs.c)seek failed in write_tables.\n");

    if ( !fwrite(&super_block_buffer, 1, BLOCK_SIZE,  fd) )
        printf("(fs.c)unable to write super_block.\n");
	else
		printf("super_block has been initialized\n");

    if ( !fwrite(&inode_map,  1, IMAPS*BLOCK_SIZE, fd) )
		printf("(fs.c)Unable to write inode_map.\n");
	else
		printf("inode_map has been initialized\n");
	
    if ( !fwrite(&zone_map,  1, ZMAPS*BLOCK_SIZE, fd) )
		printf("(fs.c)Unable to write zone_map.\n");
	else
		printf("zone_map has been initialized\n");

    if ( !fwrite( &inode_buffer, 1, INODE_BLOCKS*BLOCK_SIZE, fd) )
		printf("(fs.c)Unable to write inodes.\n");
	else
		printf("inodes has been initialized\n");
}

void init_filesystem()
{
    fd=fopen("F:\\ext2.disk","r+");
	if(!fd)
	{
		printf("Can't open ext2.disk, fsutil will try to createnew\n");
		system("fsutil file createnew F:\\ext2.disk 2147483648");	
		fd=fopen("F:\\ext2.disk","r+");
	}
	
	file_table_init();
	inode_table_init();

    setup_tables();
    super_block_init();
    //make_root_inode();
    write_tables(); 

    printf("There are:\n");
    printf("		inodes_count:%d	size:%d\n", INODES, INODE_SIZE);
    printf("		blocks_count:%d	size:%d\n", ZONES , BLOCK_SIZE);
    printf("		firstdatazone=%d (%d)\n", FIRSTZONE, NORM_FIRSTZONE);
    //printf("		Zonesize=%d\n",BLOCK_SIZE<<ZONESIZE);
}

void make_root_inode()
{
	/*
	int i;
    struct d_inode * inode = &inode_buffer[ROOT_INO];
    mark_inode(ROOT_INO);
    inode->i_zone[0] = get_free_block();
	for(i=1;i<Ext2_N_BLOCKS;i++)
		current->fs->root->i_zone[i]=0;
    inode->i_links_count = 1;
    inode->i_size = 128;
	current->fs->root->i_size=0;
	current->fs->root->i_mode= I_DIR | O_RDWR;
	current->fs->root->i_count=2;
	current->fs->root->i_dir_acl=0;
   // inode->i_mode = S_IFDIR + 0755;
   */
}

void check_blocks()
{
	/*
    unsigned int current_block=0;
    int badblocks=0;
    int try,got;
    static char buffer[BLOCK_SIZE * TEST_BUFFER_BLOCKS];
    while (current_block < ZONES)
    {
        if (lseek(fd,current_block*BLOCK_SIZE,SEEK_SET) != current_block*BLOCK_SIZE)
        printf("seek failed in check_blocks.\n");
        try = TEST_BUFFER_BLOCKS;
        if (current_block + try > ZONES)
        try = ZONES-current_block;
        got = read(fd, buffer, try * BLOCK_SIZE);
        if (got<0)
        got = 0;
        if (got & (BLOCK_SIZE-1))
        printf("Weird values in check_blocks: probably bugs\n");
        got /= BLOCK_SIZE;
        current_block += got;
        if (got == try) continue;
        if (current_block < FIRSTZONE)
        printf("bad blocks before data-area: cannot make fs\n");
        mark_zone(current_block);
        badblocks++;
        current_block++;
        }
        if (badblocks)
        printf("%d bad block%s\n",badblocks,(badblocks>1)?"s":"");
	*/
}
