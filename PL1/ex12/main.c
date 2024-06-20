#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int spawn_childs(int n) {
    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            //printf("Sou o filho %d e vou retornar este valor.\n", i+1);
            return(i + 1);
        }
    }
    //printf("Sou o pai e vou retornar 0!\n");
    return(0);
}

int main() {
    int res, status;

    res = spawn_childs(6);

    if (res > 0) {
        //printf("Sou o filho %d e vou devolver %d\n", res, res * 2);
        exit(res * 2);
    } else {
        for (int i = 0; i < 6; i++) {
            wait(&status);
            //printf("Encontrei o meu filho!\n");
        }
    }
    exit(0);
}
