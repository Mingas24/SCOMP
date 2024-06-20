#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define CHILDS 8
#define SEMS 8
#define RANGE 25

int main() {
	int i, j, numbers[RANGE * CHILDS];
	pid_t pid;

	FILE *fileIn, *fileOut;

	char* sem_names[SEMS];
	int sem_values[SEMS]; /* Lista dos valores dos semáforos */
	sem_t* sems[SEMS];

	/* Escrever os nomes dos ficheiros e os seus valores nas suas listas */
	for(i = 0; i < SEMS; i++){
		sem_names[i] = malloc(100);
		sprintf(sem_names[i], "/semaphore_%d", i);

		if(i == 0)
			sem_values[i] = 2;
		else
			sem_values[i] = 0;
	}
	for(i = 0; i < SEMS; i++)
		sem_unlink(sem_names[i]);

	/* Abrir os semáforos */
	for(i = 0; i < SEMS; i++)
		sems[i] = sem_open(sem_names[i], O_CREAT, 0644, sem_values[i]);

	/* Criar os processos filhos */
	for(i = 0; i < CHILDS; i++) {
		/* Caso dê erro a criar o filho */
		if((pid = fork()) == -1) {
			perror("Failed fork\n");
			exit(-1);
		/* Processo filho */
		} else if(pid == 0) {
			int start = i * RANGE, end = start + RANGE;
			
			sem_wait(sems[i]);
			/* abrir ficheiro de input - Numbers.txt */
			fileIn = fopen("Numbers.txt" ,"r");
			/* processo lê ficheiro de input */
            for(j = start; j < end; j++) {
				fscanf(fileIn, "%d%*c", numbers+j);
			}
			/* fecha ficheiro de texto */
			fclose(fileIn);
			/* Incrementar o semáforo do próximo processo para poder começar a leitura do ficheiro input */
            if(sem_post(sems[(i+1) % SEMS]) == -1){
				perror("Failed sem_post\n");
				exit(1);
			}

			/* Decrementa semáforo[i] - inicia processo de escrita no ficheiro output */
            sem_wait(sems[i]);
			/* abrir ficheiro de output - Output.txt */
			fileOut = fopen("Output.txt", "a");

            for(j = start; j < end; j++) {
                fprintf(fileOut, "%d \n", *(numbers + j)); /* escrever número no ficheiro */
			}

			fclose(fileOut); /* fecha ficheiro de texto */

			/* Decrementar o semáforo do próximo processo para poder começar a escrita do ficheiro output */
			if(sem_post(sems[(i+1) % SEMS]) == -1) {
				perror("Failed sem_post\n");
				exit(1);
			}

            exit(0);
		}
	}

	for(i = 0; i < CHILDS; i++)
		wait(NULL);

	for(i = 0; i < SEMS; i++)
		sem_unlink(sem_names[i]);

	return 0;
}
