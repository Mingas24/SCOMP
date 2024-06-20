#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int vec1[1000];
    int vec2[1000];
    int pids[5];
    int inicio, fim, status;
    int fd[2];
    pipe(fd);
    int tmp;
    int soma;

    srand(time(NULL));
    int i;
    int j;
    for (i = 0; i < 1000; i++) {
        vec1[i] = rand() % 10;
        vec2[i] = rand() % 10;
    }

    inicio = 0;
    fim = inicio + 200;

    for (j = 0; j < 5; j++) {
        pids[j] = fork();
        if(pids[j] == 0){
            // Começa o filho.
            for(i = inicio; i < fim; i++){
				close(fd[0]);

				tmp += vec1[i] + vec2[i];
					
                }
		
		write(fd[1],&tmp,sizeof(int));
		
		close (fd[1]);
		exit(90);	

		}
		inicio = inicio + 200;
        fim = inicio + 200;
	}
	
	for (j = 0; j < 5; j++) {
        wait(&status);
        if(WEXITSTATUS(status)) {
            read(fd[0],&tmp,sizeof(int));
            soma += tmp;
        }
    }
	
	printf("tmp total = %d",soma);
    return 0;
}
/*
Não é necessário um mecanismo de sincronização.
Cada processo filho soma e escreve num pipe a soma dos dois arrays(200 de cada vez) e no fim o pai espera que os filhos acabem para fazer a soma total.
*/
