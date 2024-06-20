#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int array2000[2000];
    int inicio, fim, status;
    pid_t pid, pids[10];

    srand(time(NULL));
    int i;
    for (i = 0; i < 2000; i++) {
        array2000[i] = rand() % 100;
    }

    int n = 10;

    inicio = 0;
    fim = inicio + 200;

    for (int j = 0; j < 10; j++) {
        pids[j] = fork();
        if(pids[j] == 0){
            // Começa o filho.
            for(i = inicio; i < fim; i++) {
                if(array2000[i] == n) {
                    exit(i - inicio);
                }
            }
            exit(255);
            // Termina o filho.
        }

        inicio = inicio + 200;
        fim = inicio + 200;
    }

    for (int j = 0; j < 10; j++) {
        pid = wait(&status);
        if(WEXITSTATUS(status) != 255) {
            for (int i = 0; i < 10; i++) {
                if(pid == pids[i]) {
                    printf("FILHO: %d\tPOSICAO: %d\n", pid, WEXITSTATUS(status) + i*200);
                    break;
                }
            }
        }
    }

    return 0;
}
