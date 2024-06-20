#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


typedef struct{
    int cod;
    char desc [100];
    float preco;
}shared_struct;

int main (){
    shared_struct *ss;
    int i;
    int r;
    int fd;     /*inteiro para o descritor da memória partilhada*/
    int cod1;
    char desc[100];
    float preco1;

    /*criação da memória*/
    fd = shm_open("/shm_test", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
    if (fd==-1){
        printf("Erro ao criar a memória partilhada\n");
        return 1;
    }

    /*define o tamanho da memória*/
    ftruncate(fd,sizeof(shared_struct));

    ss = mmap(NULL, sizeof(shared_struct),PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    /*atribui os valores às variáveis da struct*/
    printf("Insira o código do producto.\n");
    scanf("%d\n",&cod1);
    ss->cod = cod1;

    printf("Insira a descrição do producto.\n");
    scanf("%s\n", desc);
    for(i=0;i<strlen(desc);i++){
        ss->desc[i]= desc[i];
    }

    printf("Insira o preco do producto.\n");
    scanf("%f\n",&preco1);
    ss->preco = preco1;

    r = munmap(ss,sizeof(shared_struct));
    if (r < 0) exit(1); /* Check error */
    r = close(fd);
    if (r < 0) exit(1); /* Check error*/
    exit(0);
}