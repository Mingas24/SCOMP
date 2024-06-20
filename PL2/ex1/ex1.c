#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int READ = 0;
static int WRITE = 1;

int main() {
    int fd[2];

    if(pipe(fd) == -1){
        perror("Pipe error\n");
        exit(EXIT_FAILURE);
    }

    pid_t p = fork();
    if(p == -1){
        perror("Fork error\n");
        exit(EXIT_FAILURE);
    }

    if(p>0){
        close(fd[READ]);
        int p = getpid();
        printf("Dad here and my pid is: %d\n", p);
        write(fd[WRITE], &p, sizeof(p));
        close(fd[WRITE]);
    }else{
        close(fd[WRITE]);
        int ler;
        read(fd[READ], &ler, sizeof(ler));
        printf("Son here and i received: %d\n", ler);
        close(fd[READ]);
        exit(0);
    }
    return 0;
}
