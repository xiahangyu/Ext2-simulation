#include <stdio.h>
#include <stdlib.h>
#include "fs.h"
#include "task_struct.h"
#include "file_dev.h"
#include "ext2_fcntl.h"
#include "blk.h"
#include "sb.h"

void init_PCB()
{
	int i;
	current=(struct task_struct *)malloc(sizeof(struct task_struct));

	//初始化files结构
	current->files=(struct files_struct*)malloc(sizeof(struct files_struct));
	for(i=0;i<NR_OPEN;i++)
		current->files->flip[i]=NULL;

	//初始化fs结构
	current->fs=(struct fs_struct*)malloc(sizeof(struct fs_struct));
	current->fs->root=(struct m_inode *)malloc(sizeof(struct m_inode));
	current->fs->pwd=current->fs->root;
	current->fs->root->i_number=1;
	current->fs->root->i_size=0;
	current->fs->root->i_mode= I_DIR | O_RDWR;
	current->fs->root->i_count=0;
	current->fs->root->i_dir_acl=1;
	current->fs->root->i_zone[0]=get_free_block();
	for(i=1;i<Ext2_N_BLOCKS;i++)
		current->fs->root->i_zone[i]=0;

	current->fs->root->i_back=inode_table.i_back;
	current->fs->root->i_forw=&inode_table;
	inode_table.i_forw=current->fs->root;

	iput(current->fs->root);

	printf("PCB has been initialized\n");
}

int main()
{
	int myfd;
	char buffer1[20]="asdesdfsdfsdfsdf";
	char buffer2[20];

    init_filesystem();
	init_PCB();

	myfcreate("Demo.c",O_RDWR);
	myfd=myfopen("Demo.c",O_RDWR);
	
	myflseek(myfd,0,0);
	myfwrite(myfd,buffer1,20);

	myflseek(myfd,10,0);
	myfread(myfd,buffer2,20);
	printf("myfread: %s\n",buffer2);

	free(current->fs);
	free(current->files);
	free(current);
	
    return 0;
}
