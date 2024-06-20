#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include "struct.h"

#define ARRAY_SIZE 1000
#define NUMBER_RANGE 1000
#define RANGE 100
#define CHILDS 10


int main(void) {

    int array[ARRAY_SIZE]; //criação do array de 1000 inteiros

    srand((unsigned) time(NULL));

    int i;

    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % NUMBER_RANGE;
    }

    int fd, data_size = sizeof(shared_data_type);

    shared_data_type *shared_data;

    fd = shm_open("/shm_ex09", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("shm_open error\n");
        exit(-1);
    }

    if (ftruncate(fd, data_size) == -1) {
        perror("ftruncate error\n");
        exit(-1);
    }

    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid[CHILDS];

    for (i = 0; i < CHILDS; i++) {

        pid[i] = fork();

        if (pid[i] == -1) { //em caso de erro
            printf("Erro na criação do processo filho\n");
            exit(-1);
        } else if (pid[i] == 0) { //em caso de ser processo filho
            int j, max = 0;

            for (j = RANGE * i; j < RANGE * (i + 1); j++) {

                if (array[j] > max) {
                    max = array[j];
                }
            }

            shared_data->vec[i] = max;

            exit(1);
        }
    }

    for (i = 0; i < CHILDS; i++) {
        wait(NULL);
    }

    int high = 0;

    for (i = 0; i < CHILDS; i++) {

        printf("vec[%d]= %d\n", i, shared_data->vec[i]);

        if (shared_data->vec[i] > high) {
            high = shared_data->vec[i];
        }
    }

    printf("The global maximum is %d\n", high);

    if (munmap(shared_data, data_size) == -1) {
        perror("munmap error\n");
        exit(-1);
    }

    if (close(fd) == -1) {
        perror("close error\n");
        exit(-1);
    }

    return 0;

}
