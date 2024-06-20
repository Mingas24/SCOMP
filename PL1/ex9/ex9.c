//Criado por Francisco Costa 1180954
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int inicio = 1, fim = 100, status;
    pid_t p;

    for (int i = 0; i < 10; i++)
    {
        p = fork();
        if (p == 0)
        {
            for (inicio; inicio <= fim; inicio++)
            {
                printf("%d\n", inicio);
            }
            exit(0);
        }

        inicio = inicio + 100;
        fim = fim + 100;
    }
    for (int i = 0; i < 10; i++)
    {
        p = wait(&status);
        printf("[PAI] Morreu o filho %d (exit value: %d)\n", p, WEXITSTATUS(status));
    }
}