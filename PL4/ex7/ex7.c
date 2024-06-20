#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /*necessário para a utilização do fork*/
#include <stdlib.h> /*necessário para a utilização do exit*/
#include <sys/wait.h> /*necessário para o wait */
#include <fcntl.h> /* opções de controlo do ficheiro */
#include <sys/types.h> /* tipos de data usados pela API*/
#include <sys/stat.h> /* constantes usadas para abrir*/
#include <sys/mman.h> /* declarações relacionadas com a gestão da memória partilhada */
#include <semaphore.h>

#define NR_PROCESSOS 3

int main() {

    int i;
    char *filho1[2] = {"Sistemas ", "a"};
    char *filho2[2] = {"de", "melhor"};
    char *filho3[2] = {"computadores-", "disciplina"};

    /* Cria o semáforo com o valor inicial a 1*/
    sem_t *semaforo1 = sem_open("/sem1", O_CREAT, 0644, 1);
    sem_t *semaforo2 = sem_open("/sem2", O_CREAT, 0644, 0);
    sem_t *semaforo3 = sem_open("/sem3", O_CREAT, 0644, 0);

    if (semaforo1 == SEM_FAILED || semaforo2 == SEM_FAILED || semaforo3 == SEM_FAILED) {
        perror("Erro na criação dos semáforos.\n");
        exit(1);
    }

    pid_t pid;
    for (i = 0; i < NR_PROCESSOS; i++) {
        pid = fork();
        if (pid == 0 && i==0) {
            sem_wait(semaforo1);
            printf("%s\n", filho1[0]);
            sem_post(semaforo2);
            sem_wait(semaforo1);
            printf("%s\n", filho1[1]);
            sem_post(semaforo2);
            exit(0);
        }
        if(pid == 0 && i==1){
            sem_wait(semaforo2);
            printf("%s\n", filho2[0]);
            sem_post(semaforo3);
            sem_wait(semaforo2);
            printf("%s\n", filho2[1]);
            sem_post(semaforo3);
            exit(0);
        }
        if(pid == 0 && i==2){
            sem_wait(semaforo3);
            printf("%s\n", filho3[0]);
            sem_post(semaforo1);
            sem_wait(semaforo3);
            printf("%s\n", filho3[1]);
            sem_post(semaforo1);
            exit(0);
        }
    }

    /* Espera que todos os filhos terminem. */
    for (i = 0; i < NR_PROCESSOS; i++) {
        wait(NULL);
    }

    sem_unlink("/sem1");
    sem_unlink("/sem2");
    sem_unlink("/sem3");

    return 0;
}
