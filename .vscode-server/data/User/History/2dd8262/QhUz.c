#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    int p_f2c[2];//父进程向子进程写
    int p_c2f[2];//子进程向父进程写
    int pid;

    char buffer='\0';
    char wtp='a';
    char wtc='b';

    pipe(p_f2c);
    pipe(p_c2f);

    pid=fork();
    //父进程
    if(pid!=0)
    {
        close(p_f2c[0]);
        close(p_c2f[1]);

        write(p_f2c[1],&wtp,1);
        read(p_c2f[0],&buffer,1);
        if(buffer=='b')
            printf("%d: received pong\n",pid);
        exit(0);
    }
    //子进程
    else
    {
        close(p_f2c[1]);
        close(p_c2f[0]);

        read(p_f2c[0],&buffer,1);
        if(buffer=='a')
            printf("%d: received ping\n",pid);
        write(p_c2f[1],&buffer,1);
        exit(0);
    }
}