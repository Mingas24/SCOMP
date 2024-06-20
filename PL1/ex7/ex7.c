//Criado por Francisco Costa 1180954
#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n;                   /* the number to find */
    int j;
    int a;
    int status;

    time_t t; // needed to init. the random number generator (RNG)
    int i;

    // intializes RNG (srand():stdlib.h; time(): time.h)
    srand((unsigned)time(&t));

    // initialize array with random numbers (rand(): stdlib.h)
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        numbers[i] = rand() % 10000;
    }
    // initialize n
    //n = rand () % 10000;
    n = 4;

    numbers[3] = 4;
    numbers[990] = 4;

    pid_t p;

    p = fork();

    if (p == 0)
    {
        int counterF = 0;

        for (j = 0; j < 500; j++)
        {
            if (numbers[j] == n)
            {
                counterF++;
            }
        }

        exit(counterF);
    }

    if (p > 0)
    {

        int counter = 0;

        for (a = 500; a < 1000; a++)
        {
            if (numbers[a] == n)
            {
                counter++;
            }
        }

        waitpid(p, &status, 0);

        if (WIFEXITED(status))
        {

            counter = counter + WEXITSTATUS(status);

            printf("The value of n is: %d and it is found %d times\n", n, counter);
        }
    }
    return 0;
}
