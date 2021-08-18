#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    // in ls.c, code is like this:
    // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    // it's for alignment, what I need is the end of string
    buf[strlen(p)] = 0;
    return buf;
}
void find (char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if( (fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        exit(1);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return ;
    }

    switch(st.type){
        case T_FILE:
            if(strcmp(fmtname(path), name) == 0)
                printf("%s\n",path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            // add the previous path and add a slash
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                // invalid directory entry
                if(de.inum == 0)
                    continue;

                // add de.name to path
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;    
                
                // ignore "." and ".."
                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                
                find(buf, name);

            }
        break;
    }
    close(fd);

} 
int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "usage: find <path> <name>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}