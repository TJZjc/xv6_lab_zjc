#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void method(int pipe_from_parent[])
{
    int first_num;
    int pipe_to_next[2];
    int is_read;
    int num;
    int is_write=0;
    int pid;

    pipe(pipe_to_next);
    close(pipe_from_parent[1]);

    is_read=read(pipe_from_parent[0],&first_num,sizeof(first_num));
    printf("prime %d\n",first_num);

    while(is_read!=0)
    {
        is_read=read(pipe_from_parent[0],&num,sizeof(num));
        if(num%first_num!=0)
        {
            write(pipe_to_next[1],&num,sizeof(num));
            is_write=1;
        }
    }


    if(is_write==1)
    {
        pid=fork();
        if(pid==0)
        {
            method(pipe_to_next);
            exit(0);
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        exit(0);
    }
}

int main(int argc,char *argv[])
{
    int first_pipe[2];
    int pid;

    pipe(first_pipe);

    pid=fork();
    if(pid==0)//子进程
    {
        method(first_pipe);
        exit(0);
    }
    else//父进程
    {
        for(int i=2;i<=35;i++)
        {
            write(first_pipe[1],&i,sizeof(i));//将不是2的倍数的数写入管道
        }
        exit(0);
    }
}