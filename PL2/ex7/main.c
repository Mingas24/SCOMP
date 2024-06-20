#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int vec1[1000];
    int vec2[1000];
    int arrayFinal[1000];
    int fd[5][2];
    int aux, status;
    pid_t pid;

    for (int i = 0; i < 1000; ++i) {
        vec1[i] = i + 1;
        vec2[i] = 1000 - i;
    }

    for (int i = 0; i < 5; i++) {
        if (pipe(fd[i]) == -1) {
            perror("Erro no pipe!\n");
            exit(1);
        }
        pid = fork();
        if (pid == 0) {
            close(fd[i][0]);
            for (int j = i * 200; j < (i + 1) * 200; j++) {
                aux = vec1[j] + vec2[j];
                write(fd[i][1], &aux, sizeof(aux));
            }
            close(fd[i][1]);
            exit(0);
        }
    }
    for (int i = 0; i < 5; i++) {
        wait(&status);
        int posi = 200 * i;
        close(fd[i][1]);
        if (WIFEXITED(status)) {
            while (read(fd[i][0], &arrayFinal[posi], sizeof(int)) > 0) {
                posi++;
            }
        }
        close(fd[i][0]);
    }

    for (int i = 0; i < 1000; i++) {
        printf("%d ", arrayFinal[i]);
    }

    return 0;

}
