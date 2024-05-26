#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

char* readline(){
    char* buf=malloc(MAXARG);
    char* pos=buf;
    while(read(0,pos,sizeof(char))==sizeof(char)){
        if(*pos=='\n'||*pos=='\0'){
            *pos='\0';
            return buf;
        }
        pos++;

    }
    if(pos!=buf){
        return buf;
    }
    free(buf);
    return 0;
    
}

int
main(int argc, char *argv[])
{
    char* cmd;
    char* buf;
    cmd=argv[1];
    char** temp_argv=(char**)malloc(argc*sizeof(char*));
    for(int i=0;i<argc-1;++i){
        temp_argv[i]=argv[i+1];
    }

    while((buf=readline())!=0){
        //printf("count :%d\n",++count);
        temp_argv[argc-1]=buf;
        int pid=fork();
        if(pid==0){
            exec(cmd,temp_argv);
        }else{
            wait(0);
        }
            free(buf);
    }
    free(temp_argv);
    exit(0);
}