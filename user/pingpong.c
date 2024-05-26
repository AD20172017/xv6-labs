#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  int p_p2c[2];
  int p_c2p[2];
  char buf[]={"Hello world!\n"};
  pipe(p_c2p);
  pipe(p_p2c);
  int pid=fork();
  if(pid>0){
    //parent
    int c_state;
    close(p_c2p[1]);
    close(p_p2c[0]);
    write(p_p2c[1],&buf,1);
    read(p_c2p[0],&buf,1);
    wait(&c_state);
    fprintf(1,"%d: received pong\n",getpid());
    close(p_c2p[0]);
    close(p_p2c[1]);
  }else if(pid==0){
    close(p_c2p[0]);
    close(p_p2c[1]);
    write(p_c2p[1],&buf,1);
    read(p_p2c[0],&buf,1);
    fprintf(1,"%d: received ping\n",getpid());
    close(p_c2p[1]);
    close(p_p2c[0]);
  }else{
    fprintf(2,"fork error\n");
    exit(1);
  }
  exit(0);
}