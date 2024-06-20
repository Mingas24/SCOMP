#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define n_sems 2
#define num_filh 1

int create_childs(int n_filhos)
{
    pid_t pid;
    int i;
    for (i = 0; i < n_filhos; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Erro no fork!\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            return i + 1;
        }
    }

    return 0;
}

int main()
{
    sem_t *sem[n_sems];
    char sems_names[][25] = {"/semaforo_filho_ex5", "/semaforo_pai_ex5"};
    int sems_init_vales[] = {1, 0};
    enum index
    {
        SON,
        FATHER
    };

    // Inicializar semáforos
    int i;
    for (i = 0; i < n_sems; i++)
    {
        if ((sem[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, sems_init_vales[i])) == SEM_FAILED)
        {
            perror("No sem_open()");
            exit(1);
        }
    }

    // Criar processos filho
    int id = create_childs(num_filh); // Pai = 0; Filhos > 0

    // Código do processo filho
    if (id > 0)
    {
        sem_wait(sem[SON]);

        printf("I'm the child\n");

        sem_post(sem[FATHER]);
    }
// 
    // Código do processo pai
    if (id == 0)
    {
        sem_wait(sem[FATHER]);

        printf("I'm the father\n");

        sem_post(sem[SON]);
    }

    // Fechar os semáforos
    sem_close(sem[SON]);
    sem_close(sem[FATHER]);

    // Remover ficheiros dos semáforos
    sem_unlink(sems_names[SON]);
    sem_unlink(sems_names[FATHER]);

    return 0;
}