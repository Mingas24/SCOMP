//Criado por Francisco Costa 1180954
#define ARRAYSIZE = 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int numbers[ARRAYSIZE];
    time_t t;

    srand((unsigned)time(&t));

    for (int i = 0; i < ARRAYSIZE; i++)
    {
        numbers[i] = rand() % 255;
    }

    int max_value = 0, inicio = 0, fim = 199;
    pid_t p;

    for (int i = 0; i < 5; i++)
    {
        p = fork();
        if (p == 0)
        {
            for (inicio; inicio <= fim; inicio++)
            {
                int aux = numbers[inicio];
                if (max_value < aux)
                {
                    max_value = aux;
                }
            }
            exit(0);
        }
        inicio = inicio + 200;
        fim = fim + 200;
    }

    int result[ARRAYSIZE];

    int status;
    pid_t pid = fork;

    if(pid==0){
        for(i=0; i<ARRAYSIZE/2; i++){
            result[i] = (numbers[i]/max_value)*100;
            printf("Index: %d Result: %d", i, result[i]);
        }
        exit(0);
    }
    if(pid>0){
        wait(&status);
        for(i = ARRAYSIZE/2; i<ARRAYSIZE;i++){
            result[i] = (numbers[i]/max_value)*100;
            printf("Index: %d Result: %d", i, result[i]);
        }
    }
    return 0;
}