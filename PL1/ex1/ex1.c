
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

//Assume that the PID of the child process is 1234.

int main(void) {
    int x = 1;
    pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
    if (p == 0) {
        x = x+1;
        printf("1. x = %d\n", x);
    } else {
        x = x-1;
        printf("2. x = %d\n", x);
    }
    printf("3. %d; x = %d\n", p, x);
}

/*
 * Imprime:     2. x = 0
 *              3. 1234; x = 0
 *
 *              quando executa o processo pai
 *              o valor do PID que é atribuido a p, é o valor do PID do processo filho criado.
 *              Neste caso, 1234.
 *
 * Imprime:     1. x = 2;
 *              3. 0; x = 2
 *
 *              quando executa o processo filho
 *              O valor do PID que é atribuido a p, é o valor 0 porque é o valor que o PID filho vê


 b) Para o mesmo processo, sim.
    Neste caso, para o processo pai, o printf da linha 9 acontece sempre antes do printf
    da linha 11. Agora, é possível ver primeiro o printf da linha 11 caso esteja a ser
    executado o processo filho. 

    */
