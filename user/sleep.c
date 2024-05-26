#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  int i;
  if(argc>2){
    fprintf(2, "need more \n");
    exit(1);
  }     
  if(argc==1){
    i=20;
  }else {
    i=atoi(argv[argc]);
  }
  fprintf(2, "sleeping \n");

  sleep(i);
  exit(0);
}