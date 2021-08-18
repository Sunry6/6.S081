#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_sieve(int input_pd)
{
    int pd[2];
    int prime;

    if(read(input_pd, &prime, sizeof(int)) > 0){
        printf("prime %d\n",prime);
        pipe(pd);
        int pid;
        if((pid = fork()) == 0){
            close(pd[1]);
            prime_sieve(pd[0]);
        }else{            
            int number;
            while(read(input_pd, &number, sizeof(int)) > 0){
                if(number % prime != 0){    
                    write(pd[1], &number, sizeof(int));
                }
            }
            close(pd[0]);
            close(pd[1]);
            wait(0);
        }
    }
    exit(0);
}
int main()
{
    int pd[2];
    pipe(pd);
    // initial
    if(fork() == 0){    // child
        close(pd[1]);
        prime_sieve(pd[0]);
    }else{              // parent
        close(pd[0]);
        for(int num=2; num<=35; num++){
            write(pd[1], &num, sizeof(int));
        }
        close(pd[1]);
        wait(0);
    }
    exit(0);
}
