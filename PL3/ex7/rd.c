#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
     int numbers[10];
}shared_array; 


int main(int argc, char* argv[]){

    int average = 0;
    int i, r;
    int fd;     /*inteiro para o descritor da memoria partilhada*/
    int numbers[10];

    shared_array *pt;

    /*abre a área partilhada com permissão de read e write*/
    fd = shm_open("/shmtest", O_RDWR,S_IRUSR|S_IWUSR);


    ftruncate(fd, sizeof(shared_array));

    pt = (shared_array*) mmap(NULL, sizeof(shared_array),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    /*percorrer o array para calcular a media*/
    for(i = 0; i < 9; i++){
        average = average + pt->numbers[i];

    }

    for(i = 0; i < 9; i++){
        printf("%d\n", pt->numbers[i]);

    }

    printf("Average:    %d\n", average/10);

    r = munmap(pt, 20);
    if (r < 0) exit(1); /* Check error */

    r = shm_unlink("/shmtest"); /* removes */
    if (r < 0) exit(1); /* Check error*/
    exit(0);

}
