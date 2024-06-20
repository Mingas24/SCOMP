#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

typedef struct{
    long num;
}shared_struct;

int main(void){

    int fd; /*descritor para a memória partilhada*/
    int i, r;

    shared_struct array[1000000];        /*guarda o que ler da memória partilhada*/
    shared_struct *pt;

    /*tempos em que começa a leitura*/
    clock_t shm_t;
    clock_t pipe_t;

    /*tempo que durou a leitura*/
    clock_t shm_duration;
    clock_t pipe_duration;

        //Shared memory

    /*criar e abrir área de memória partilhada*/
    fd = shm_open("/ex6",O_RDWR,S_IRUSR|S_IWUSR);

    /*verifica se a memória partilhada foi criada com sucesso*/
    if(fd == -1){
        printf("### Failed to create shared memory ###");
        return 1;
    }

    /*define tamanho do bloco de memória partilhada*/
    ftruncate(fd, sizeof(array));

    /*mapeamento da memória partilhada, na memória do processo*/
    pt = (shared_struct *)mmap(NULL, sizeof(array),PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    /*Começar o timer*/
    shm_t = clock();

    /*preenche o array do processo com os dados
    do array que está na memória partilhada*/
    for(i = 0; i < 1000000; i++){
        array[i].num=pt->num;
    }

    shm_duration = clock() - shm_t;

    /*imprime o tempo que demorou a ler*/
    printf("tempo de leitura:   %ldms\n\n", shm_duration);

    r = munmap(pt, sizeof(shared_struct));
    if (r < 0) exit(1); /* Check error */

    r = shm_unlink("/ex6"); /* removes */
    if (r < 0) exit(1); /* Check error*/
    exit(0);

}