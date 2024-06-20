#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define NUM_CHILDS 2 /* número de processos filho */
#define SEM_NAME_1 "semaforo1_ex9" /* nome do primeiro semáforo */
#define SEM_NAME_2 "semaforo2_ex9" /* nome do segundo semáforo */

int main() {
	int i;
	sem_t *sem1, *sem2;
	
	/* criar semáforos */
	if((sem1 = sem_open(SEM_NAME_1, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem2 = sem_open(SEM_NAME_2, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	/* criar processos filho */
	for(i = 0; i < NUM_CHILDS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) { 
			if(i == 0) { /* processo 1 */
				printf("Child 1 - buy chips\n"); /* compra batatas fritas */
			
				sem_post(sem2); /* segundo processo pode avançar */
			
				sem_wait(sem1); /* primeiro processo fica à espera */
			
				printf("Child 1 - eat and drink\n"); /* bebe e come */
				
				exit(0);
			}
			if(i == 1) { /* processo 2 */
				printf("Child 2 - buy beer\n"); /* compra cerveja */
			
				sem_post(sem1); /* primeiro processo pode avançar */
			
				sem_wait(sem2); /* segundo processo fica à espera */
			
				printf("Child 2 - eat and drink\n"); /* bebe e come */
				
				exit(0);
			}
		}
	}
	
	/* processo pai espera pelos processos filho */
	for(i = 0; i < NUM_CHILDS; i++) {
		wait(NULL);
	}
	//fechar os semaforos
    sem_close(SEM_NAME_1);
    sem_close(SEM_NAME_2);
    
	// remover os ficheiros 
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	
	return 0;
}