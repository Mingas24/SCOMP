#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define shm_name "/shm_ex12"
#define STR_SIZE 50
#define NR_DISC 10

typedef struct
{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
} aluno;

typedef struct
{
    int new_data_bool;
    aluno student;
} shared_data_type;

int calc_avg_grade(int *grades)
{
    int i, total = 0;
    for (i = 0; i < NR_DISC; i++)
    {
        total += grades[i];
    }
    return (total / NR_DISC);
}

int calc_min(int *grades){
    int i, min = 999;
    for (i = 0; i < NR_DISC; i++)
    {
        if(grades[i] < min){
            min = grades[i];
        }
    }

    return min;
}

int calc_max(int *grades)
{
    int i, max = 0;
    for (i = 0; i < NR_DISC; i++)
    {
        if(grades[i] > max){
            max = grades[i];
        }
    }

    return max;
}

int main(int argc, char *argv[])
{

    pid_t pid;
    int data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    // Criação do processo filho e sua validação
    pid = fork();
    if (pid == -1)
    {
        perror("Erro");
    }

    // Código do processo pai
    if (pid > 0)
    {
        // Preencher os dados com informação dada pelo utilizador
        // Número do aluno
        printf("Número do aluno: ");
        scanf("%d%*c", &(shared_data->student.numero));

        // Nome do aluno
        printf("Nome do Aluno: ");
        fgets(shared_data->student.nome, STR_SIZE, stdin);
        int lenght = strlen(shared_data->student.nome);
        if (shared_data->student.nome[lenght - 1] == '\n' || shared_data->student.nome[lenght - 1] == '\r')
        {
            shared_data->student.nome[lenght - 1] = '\0';
        }

        // Notas nas disciplinas
        int i;
        for (i = 0; i < NR_DISC; i++)
        {
            printf("Nota da disciplina %d: ", i);
            scanf("%d%*c", &(shared_data->student.disciplinas[i]));
        }

        shared_data->new_data_bool = 1;

        // Esperar pelo processo filho
        wait(NULL);

        if (munmap(shared_data, 100) < 0)
        {
            exit(1);
        }

        if (shm_unlink(shm_name) < 0)
        {
            exit(1);
        }
    }

    // Código do processo filho
    if (pid == 0)
    {
        while (!shared_data->new_data_bool)
            ;
        int avg = calc_avg_grade(shared_data->student.disciplinas);
        int max = calc_max(shared_data->student.disciplinas);
        int min = calc_min(shared_data->student.disciplinas);
        printf("A média do aluno %s (%d) é: %d\n", shared_data->student.nome, shared_data->student.numero, avg);
        printf("A nota máxima do aluno %s (%d) é: %d\n", shared_data->student.nome, shared_data->student.numero, max);
        printf("A nota minima do aluno %s (%d) é: %d\n", shared_data->student.nome, shared_data->student.numero, min);

        exit(0);
    }

    return 0;
}