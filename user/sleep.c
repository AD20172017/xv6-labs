#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if(argc>2){
    fprintf(2,"usage:sleep need two argv!\n");
    exit(-1);
  }
  int time=atoi(argv[1]);
  if(time<0){
    fprintf(2,"usage:time mush greater than 0!\n");
    exit(-1);
  }
  sleep(time);
  // \n 必须有
  fprintf(1,"sleep finished!\n");
  exit(0);
}
