#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("I'm..\n");
    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
    printf("the..\n");
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
    printf("father!\n");
    pid_t pid3 = fork();
    if (pid3 == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
}
