#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("There is no parameters!");
    }
    else
    {
        int time=atoi(argv[1]);
        sleep(time);
    }
    exit(0);
}