#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) 
{
  int i, count = 0, read_size, buf_ptr = 0;
  char* args[MAXARG],*arg;
  char block[32], buf[32];
  arg=buf;

  for (i = 1; i < argc; i++) //根据输入的参数得出要执行的函数以及对应的参数
  {
    args[count] = argv[i];
    count++;
  }

  while ((read_size = read(0, block, sizeof(block))) > 0) //只要未按下ctrl+d，就一直读取要传入此函数的参数
  {
    for (i = 0; i < read_size; i++) //在标准输入中读到内容
    {
      if (block[i] == '\n') 
      {
        buf[buf_ptr] = 0;
        args[count] = arg;
        count++;
        args[count]=0;
        buf_ptr = 0;
        arg=buf;
        count = argc - 1;

        if (fork() == 0) //子进程执行函数
        {
          exec(argv[1], args);
        }
        else//父进程等待子进程执行完毕
        {
          wait(0);
        }
      } 
      else if (block[i] == ' ') //读到空格，则将buf中的参数传入保存的数组
      {
        buf[buf_ptr] = 0;
        args[count] = arg;
        buf_ptr++;
        count++;
        arg=&buf[buf_ptr];
      } 
      else 
      {
        buf[buf_ptr] = block[i];
        buf_ptr++;
      }
    }
  }
  exit(0);
}