#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argv, char *argc[])
{
    char *command = argc[1];
    char *argc_1[MAXARG];
    // argc -- xargs echo bye
    // argc_1 -- echo bye
    // command  --  echo
    int index = 0;
    for(int i = 1; i < argv; i++){
        argc_1[index++] = argc[i];
    }

    char read_line[MAXARG];     // param.h   -->   #define MAXARG    32  // max exec arguments
    int n;
    while((n = read(0, read_line, sizeof(read_line))) > 0){
        if(fork() == 0){
            char *tmp = (char *)malloc(sizeof(read_line));
            int t = 0;
            for(int i = 0; i < n; i++){
                if(read_line[i] != ' ' && read_line[i] != '\n'){
                    tmp[t++] = read_line[i];
                }else{
                    tmp[t] = '\0';
                    argc_1[index++] = tmp;
                    t = 0;    // ****
                    tmp = (char *)malloc(sizeof(read_line));
                }
            }
            
            argc_1[index] = 0;
            exec(command, argc_1);
            fprintf(2, "exec failed\n");
            exit(1);
        }else{
            wait(0);
        }
    }
    exit(0);
}