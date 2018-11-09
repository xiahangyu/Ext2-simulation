#ifndef TASK_STRUCT_H_INCLUDED
#define TASK_STRUCT_H_INCLUDED

#include"fs.h"

struct task_struct{
    int pid;
    int pgid;

    int uid;
    int gid;

    struct fs_struct *fs;
    struct files_struct *files;
};

struct task_struct *current;

#endif // TASK_STRUCT_H_INCLUDED
