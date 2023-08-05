#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void 
find(char *path, char *fileName) {
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;
  if ((fd = open(path, 0)) < 0 ) {//文件打开失败
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {//文件属性错误
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  //printf("%s %s\n",path, fmtname(path));
  switch(st.type) {
  case T_FILE://如果是一个文件
    //printf("%s\n", fmtname(path));
    if (strcmp(fmtname(path), fileName) == 0) {//如果文件名匹配
      printf("%s\n", path);
    }
    break;
  case T_DIR://如果是一个目录
    //printf("%s %s\n", path, fmtname(path));
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      find(buf, fileName);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("error\n");
    exit(1);
  }
  else
  {
    find(argv[1], argv[2]);
    exit(0);
  }
}