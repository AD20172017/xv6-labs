#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//make GRADEFLAGS=primes grade
int prime(int p[])
{

    close(p[1]);
    int buf;
    if (read(p[0], &buf, 4) > 0)
    {
        fprintf(1, "prime %d\n", buf);
        int p1[2];
        pipe(p1);

        int temp;
        int pid = fork();
        if (pid == 0)
        {
            close(p[0]);
            prime(p1);
        }
        else
        {
            while (read(p[0], &temp, 4))
            {

                if (temp % buf == 0)
                {
                }
                else
                {

                    close(p1[0]);
                    write(p1[1], &temp, 4);
                }
            }close(p1[1]);//!!!!close wait 的顺序？？
            wait(0);
        }
        
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int i = 3;
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid != 0)
    {
        fprintf(1, "prime 2\n");
        while (i <= 35)
        {
            if (i % 2 == 0)
            {
            }
            else
            {
                close(p[0]);
                write(p[1], &i, 4);
            }
            i++;
        }
        close(p[1]);
        wait(0); //不用wait的话会导致程序结果顺序混乱
    }
    else
    {   
        prime(p);
    }
    exit(0);
}