#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    int i;
    int status;

    pid_t pid[4];

    for (i = 0; i < 4; i++) {
        pid[i] = fork();

        if (pid[i] == 0) {
            // Este bloco tem TODO o código do filho.
            // O filho não sai deste bloco por causa do exit.
            sleep(1);
            exit(i + 1);
        }

        printf("PAI: acabei de criar o filho %d\n", pid[i]);
    }


    for (i = 0; i < 4; i++) {
        if (pid[i] % 2 == 0) {
            pid_t child = waitpid(pid[i], &status, 0);
            printf("O filho com pid: %d teminou (exit value: %d)\n", child, WEXITSTATUS(status));
        }
    }

    printf("This is the end.\n");
    return 0;
}