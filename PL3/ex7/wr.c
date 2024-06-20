#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


typedef struct{
     int numbers[10];
}shared_array;


int main(){

    int i;
    int r;
    int fd;     /*inteiro para o descritor da memória partilhada*/
    int numbers[10];
    
    shared_array *pt;


    /*criar e abrir área de memória partilhada*/
    fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

    /*verifica se a memória partilhada foi criada com sucesso*/
    if(fd == -1){
        printf("### Failed to create shared memory ###");
        return 1;
    }


    ftruncate(fd, sizeof(shared_array));

    pt = (shared_array*) mmap(NULL, sizeof(shared_array),PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    srand(time(0));

    /*preenche o array com inteiros random entre 1 e 20*/
    for(i = 0; i < 9; i++){
        pt->numbers[i] = rand() % (20 - 10 + 1) + 10; 

    }

    for(i = 0; i < 9; i++){
        printf("%d\n", pt->numbers[i]);

    }

    r = munmap(pt, 20);
    if (r < 0) exit(1); /* Check error */

    r = close(fd);
    if (r < 0) exit(1); /* Check error*/
    exit(0);


}
