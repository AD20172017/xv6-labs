#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int p[2]){
  close(p[1]);
  int num;
  //
  int r=read(p[0],&num,sizeof(int));
  //fprintf(1,"read return %d",r);
  if(r!=sizeof(int))exit(0);
  fprintf(1,"prime %d\n",num);
  int new_p[2];
  pipe(new_p);
  int n;
  while(read(p[0],&n,sizeof(int))==sizeof(int)){
    if(n%num!=0){
     // fprintf(1,"debug: n %d\n",n);
      write(new_p[1],&n,sizeof(int));
    }
  }
  close(p[0]);
  close(new_p[1]);
  //数据转移完毕
  if(fork()>0){
    close(new_p[0]);
    wait(0);
  }else{
    prime(new_p);
  }


}

int
main(int argc, char *argv[])
{
  int p[2];
  
  pipe(p);
  for(int i=2;i<=35;++i){
        write(p[1],&i,sizeof(int));
    }
  int pid=fork();
  if(pid>0){
    close(p[0]);
    
    close(p[1]);
    wait(0);

  }else if(pid==0){
    prime(p);
    
    
  }
  exit(0);
}