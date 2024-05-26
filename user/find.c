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
  return buf;
}

void find(char * dir, char * path){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", dir);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", dir);
    close(fd);
    return;
  }
  switch(st.type){
  
  case T_DIR:
  
  
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, dir);
    //在buf后面加/
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

      if(strcmp(p,".")==0||strcmp(p,"..")==0)
        continue;
    if(st.type==T_DIR)
      find(buf,path);
    else if(strcmp(path,p)==0)
      printf("%s\n",buf);
        
      //if(strcmp(fmtname(buf),path)==0)
        //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);

}

int
main(int argc, char *argv[])
{
  

  if(argc != 3){
    fprintf(2,"error\n");
    exit(1);
  }
  find(argv[1],argv[2]);
  exit(0);
}