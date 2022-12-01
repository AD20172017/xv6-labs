#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
// make GRADEFLAGS=xargs grade
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }

    char *temp_argv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        temp_argv[i - 1] = argv[i];
    }

    char buf[512];
    char temp;
    int state = 1;

    while (state == 1)
    {
        int buf_rear = 0;
        int arg_front = 0;
        int argv_rear = argc - 1;
        while (1)
        {
            state = read(0, &temp, 1);
            if (state == 0)
            {
                exit(0);
            }
            if (temp == ' ' || temp == '\n')
            {
                buf[buf_rear++] = 0;
                temp_argv[argv_rear++]=&buf[arg_front];
                arg_front=buf_rear;//指向每个命令输入的开始
                if (temp == '\n') break;
            }
            else
            {
                buf[buf_rear++] = temp;
            }
        }
        temp_argv[argv_rear]=0;
        if(fork()==0){
            exec(temp_argv[0],temp_argv);
        }else{
            wait(0);
        }
    }
    exit(0);
}