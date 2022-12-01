#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
// make GRADEFLAGS=find grade

char *
fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  // if (strlen(p) >= DIRSIZ)
  //   return p;
  // memmove(buf, p, strlen(p));
  // memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));//对齐
  // return buf;
  memmove(buf, p, strlen(p) + 1);
  return buf;
}

int find(char *path, char *file)
{
  int findNum = 0;
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  //地址能否正确
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return 0;
  }
  //文件状态是否正常
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return 0;
  }
  //文件类型
  switch (st.type)
  {
  case T_FILE:
    if (strcmp(fmtname(path), file) == 0)
    {
      //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
      printf("%s\n", path);
      findNum++;
    }

    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[strlen(de.name)] = 0; // yi chuo dian
      if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
        continue;
      if (stat(buf, &st) < 0)
      {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      // if (strcmp(fmtname(de.name), file) == 0){
      // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);}
      findNum = findNum + find(buf, file);
    }
    break;
  }
  close(fd);
  return findNum;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(2, "error argc number\n");
    exit(0);
  }
  int ret = find(argv[1], argv[2]);
  if (ret == 0)
  {
    printf("find: cannot find %s in %s\n", argv[2], argv[1]);
  }
  exit(0);
}
