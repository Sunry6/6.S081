#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int fd1[2];     // parent -> child
    int fd2[2];     // child  -> parent
    pipe(fd1);
    pipe(fd2);
    int pid = fork();
    if(pid == 0){
        char buf[100];
        read(fd1[0], buf, sizeof(buf));
        write(fd2[1], "pong", 4);
        close(fd2[1]);
        printf("%d: received %s\n", pid, buf);
        exit(0);
    }else{
        char buf[100];
        write(fd1[1], "ping", 4);
        close(fd1[1]);
        wait(0);
        read(fd2[0], buf, sizeof(buf));
        printf("%d: received %s\n", pid, buf);
        exit(0);
    }
    exit(0);
}
