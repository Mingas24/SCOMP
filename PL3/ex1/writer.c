#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "student.h"

int main(int argc, char* argv[] ){
    estudante *aluno;
    int fd, data_size = sizeof(estudante);

    fd = shm_open("/shmtest", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("Erro na criação da memória partilhada.\n");
        exit(1);
    }

    if(ftruncate(fd, data_size) == -1){
        perror("Erro no truncate.\n");
        exit(1);
    }

    aluno = (estudante*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Número: ");

    scanf("%d%*c", &(aluno->numero));
    printf("Nome: ");

    fgets(aluno->nome, 200, stdin);
    int length = strlen(aluno->nome);

    if((aluno->nome[length] == '\n') || (aluno->nome[length] == '\r')){
        aluno->nome[length] ='\0';
    }

    if(munmap(aluno, data_size) == -1){
        perror("Erro no munmap.\n");
        exit(1);
    }

    if(close(fd) == -1){
        perror("Erro no close.\n");
        exit(1);
    }

    return 0;
}