#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct{
    int cod;
    char desc [100];
    float preco;
}shared_struct;

int main() {

    shared_struct *ss;
    int fd,r;

    /*abre a área partilhada com permissão de read e write*/
    fd = shm_open("/shm_test", O_RDWR,S_IRUSR|S_IWUSR);

    ftruncate(fd, sizeof(shared_struct));

    ss = (shared_struct*) mmap(NULL, sizeof(shared_struct),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    printf("O código do produto é %d, a descrição é %s e custa %f",ss->cod,ss->desc,ss->preco);

    r = munmap(ss,sizeof(shared_struct));
    if (r < 0) exit(1); /* Check error */

    r = shm_unlink("/shm_test"); /* removes */
    if (r < 0) exit(1); /* Check error*/
    exit(0);
}
