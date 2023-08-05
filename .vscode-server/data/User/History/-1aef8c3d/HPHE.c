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
    close(pipe_to_next[0]);

    is_read=read(pipe_from_parent[0],&first_num,sizeof(first_num));
    printf("prime %d\n",first_num);

    while((is_read))
    {
        is_read=read(pipe_from_parent[0],&num,sizeof(num));
        if(num%first_num!=0)
        {
            write(pipe_to_next[1],&num,sizeof(num));
            is_write=1;
        }
        printf("2");
    }

    if(is_write)
    {
        pid=fork();
        if(pid==0)
        {
            method(pipe_to_next);
        }
        exit(0);
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
    int a;

    pipe(first_pipe);

    //close(first_pipe[0]);
    pid=fork();
    if(pid==0)
    {
        //method(first_pipe);
        exit(0);
    }
    else
    {
        for(int i=2;i<=35;i++)
        {
            write(first_pipe[1],&i,sizeof(i));
            read(first_pipe[0],&a,sizeof(a));
            printf("%d",a);
        }
        exit(0);
    }
}