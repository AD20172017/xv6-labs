#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// 1. 如果所有指向管道写端的文件描述符都关闭了（管道写端引用计数为0），而仍然有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会返回0，就像读到文件末尾一样。

// 2. 如果有指向管道写端的文件描述符没关闭（管道写端引用计数大于0），而持有管道写端的进程也没有向管道中写数据，这时有进程从管道读端读数据，那么管道中剩余的数据都被读取后，再次read会阻塞，直到管道中有数据可读了才读取数据并返回。

// 3. 如果所有指向管道读端的文件描述符都关闭了（管道读端引用计数为0），这时有进程向管道的写端write，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。当然也可以对SIGPIPE信号实施捕捉，不终止进程。具体方法信号章节详细介绍。

// 4. 如果有指向管道读端的文件描述符没关闭（管道读端引用计数大于0），而持有管道读端的进程也没有从管道中读数据，这时有进程向管道写端写数据，那么在管道被写满时再次write会阻塞，直到管道中有空位置了才写入数据并返回。

// 总结：

// ① 读管道： 1. 管道中有数据，read返回实际读到的字节数。

// 2. 管道中无数据：

// (1) 管道写端被全部关闭，read返回0 (好像读到文件结尾)

//   (2) 写端没有全部被关闭，read阻塞等待(不久的将来可能有数据递达，此时会让出cpu)

//     ② 写管道： 1. 管道读端全部被关闭， 进程异常终止(也可使用捕捉SIGPIPE信号，使进程不终止)

// 2. 管道读端没有全部关闭：

// (1) 管道已满，write阻塞。

// (2) 管道未满，write将数据写入，并返回实际写入的字节数。
int
main(int argc, char *argv[]){
int parent[2],child[2];
// char *argv[2];
// argv[0]="ping";
// argv[1]="pong";
pipe(parent);
pipe(child);
char buf[1];

if(fork()==0){
    //child
    //close(0);
    //dup(parent[0]);
    //close(parent[0]);
    //关闭管道的写端
    close(parent[1]);
    close(child[0]);
    if(read(parent[0],buf,1)){
        fprintf(1,"%d:received ping\n",getpid());
    }
    write(child[1],"B",1);
    close(child[1]);
}else{
    close(child[1]);
    close(parent[0]);
    write(parent[1],"A",1);
    close(parent[1]);
    if(read(child[0],buf,1)){
        fprintf(1,"%d:received pong\n",getpid());
    }

}
exit(0);

}