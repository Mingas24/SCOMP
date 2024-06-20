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

#define shm_name "/shm_ex03"
#define sem_name "/sem_ex03"
#define num_strings 50
#define string_size 80
#define num_childs 10

typedef struct
{
    char nome[num_strings][string_size];
    int write_index;
} shared_data_type;

int create_childs(int qtd_filhos)
{
    pid_t pid;
    int i;
    for (i = 0; i < qtd_filhos; i++)
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
    sem_t *sem;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    // Inicializar memória partilhada
    fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Erro ao abrir a memoria!\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, data_size) == -1)
    {
        perror("Erro a definir o tamanho!\n");
        exit(EXIT_FAILURE);
    }

    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Erro no mmap!\n");
        exit(EXIT_FAILURE);
    }

    shared_data->write_index = 0;

    // Inicializar semáforo
    if ((sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("No sem_open()");
        exit(1);
    }

    // Criar processos filho
    int id = create_childs(num_childs); // Pai = 0; Filhos > 0

    sem_wait(sem);

    if (shared_data->write_index >= num_strings)
    {
        shared_data->write_index = 0;
    }
    sprintf(shared_data->nome[shared_data->write_index], "I have the PID %d", getpid());
    shared_data->write_index++;

    //sleep(1);

    sem_post(sem);

    // Código do processo pai
    if (id == 0)
    {
        int i;
        // Esperar que todos os filhos terminem
        for (i = 0; i < num_childs; i++)
        {
            wait(NULL);
        }
        
        // Imprimir o conteúdo da memória partilhada
        for (i = 0; i < num_strings; i++)
        {
            printf("%s\n", shared_data->nome[i]);
        }
    }

    // Fechar a memória partilhada
    if (munmap(shared_data, 100) < 0)
    {
        exit(1);
    }

    if (shm_unlink(shm_name) < 0)
    {
        exit(1);
    }

    // Fechar os semáforos
    sem_close(sem);
    sem_close(sem);

    // Remover ficheiros dos semáforos
    sem_unlink(sem_name);
    sem_unlink(sem_name);

    return 0;
}