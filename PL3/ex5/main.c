#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct{
    int primeiroNumero;
    int segundoNumero;
}Numeros;

int main(int argc, char* argv[]){
    int fd;
    int data_size = sizeof(Numeros);
    Numeros *num;
    fd = shm_open("/ex05", O_CREAT | O_EXCL | O_RDWR , S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("Erro ao criar a memoria \n");
        exit(EXIT_FAILURE);
    }
    if(ftruncate(fd,data_size) == -1){
        perror("Erro a definir o tamanho \n");
        exit(EXIT_FAILURE);
    }
    num = (Numeros *) mmap (NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    if(num == MAP_FAILED){
        perror("Erro no mmap!\n");
        exit(EXIT_FAILURE);
    }
    num->primeiroNumero = 200;
    num->segundoNumero = 8000;

    pid_t pid = fork();
    if(pid == -1){
        perror("Erro no fork!\n");
        exit(EXIT_FAILURE);
    }
    if( pid== 0){
        int i;
        for (i=0; i<1000000; i++){
            int num1 = num->primeiroNumero;
            num1 ++;
            int num2 = num->segundoNumero;
            num2 --;
            num->primeiroNumero = num1;
            num->segundoNumero = num2;
        }
        exit(0);
    }else{
        wait (NULL);
        int j;
        for (j=0; j<1000000; j++){
            int num3 = num->primeiroNumero;
            num3 --;
            int num4 = num->segundoNumero;
            num4 ++;
            num->primeiroNumero = num3;
            num->segundoNumero = num4;
        }
    }
    printf("Primeiro Número: %d\n", num-> primeiroNumero);
    printf("Segundo Número: %d\n", num-> segundoNumero);
    if(munmap(num,data_size) == -1){
        perror("Erro no munmap! \n");
        exit(EXIT_FAILURE);
    }
    shm_unlink("/ex05");

    return 0;
}
