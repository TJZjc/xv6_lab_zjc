#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) 
{
  int i, count = 0, read_size, buf_ptr = 0;
  char* args[MAXARG],*arg;
  char block[32], buf[32];
  arg=buf;

  for (i = 1; i < argc; i++) 
  {
    args[count] = argv[i];
    count++;
  }

  while ((read_size = read(0, block, sizeof(block))) > 0) 
  {
    for (i = 0; i < read_size; i++) 
    {
      if (block[i] == '\n') 
      {
        buf[buf_ptr] = 0;
        args[count] = arg;
        count++;
        args[count]=0;
        buf_ptr = 0;
        arg=buf;

        if (fork() == 0) 
        {
          exec(argv[1], args);
        }
        else
        {
          wait(0);
        }
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