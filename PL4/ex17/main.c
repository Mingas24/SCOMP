#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define NUM_VISITORS 305 /* número de processos filho */
#define MAX_VISITORS 300 /* quantidade máxima de visitantes */

#define VIP_ID 0 /* identificador de um visitante VIP */
#define SPECIAL_ID 1 /* identificador de um visitante Special */
#define NORMAL_ID 2 /* identificador de um visitante Normal */

#define SEM_NAME_1 "semaforoVIP_ex17" /* nome do primeiro semáforo */
#define SEM_NAME_2 "semaforoSP_ex17" /* nome do segundo semáforo */
#define SEM_NAME_3 "semaforoN_ex17" /* nome do terceiro semáforo */
#define SEM_NAME_4 "semaforoV_ex17" /* nome do quarto semáforo */

int main() {
	
	int i, id, numVip = 0, numSpecial = 0;
	
	sem_t *semVip; /* controla a entrada de visitantes VIP */
	sem_t *semSpecial; /* controla a entrada de visitantes Special */
	sem_t *semNormal; /* controla a entrada de visitantes Normal */
	sem_t *semVisitors; /* controla a capacidade restante do cinema */
	
	/* criar semáforos */
	if((semVip = sem_open(SEM_NAME_1, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((semSpecial = sem_open(SEM_NAME_2, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((semNormal = sem_open(SEM_NAME_3, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((semVisitors = sem_open(SEM_NAME_4, O_CREAT | O_EXCL, 0644, MAX_VISITORS)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	for(i = 0; i < NUM_VISITORS; i++) {
		if(i % 3 == VIP_ID)
			numVip++; /* aumenta o número de VIPs na fila de espera */
		if(i % 3 == SPECIAL_ID)
			numSpecial++; /* número de Specials na fila de espera aumenta */
	}
	
	for(i = 0; i < NUM_VISITORS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) {
			while(1) {
				id = i % 3; /* atribuição de uma classificação ao visitante (VIP, Special, Normal) */
				
				if(id == VIP_ID) { /* se o visitante for VIP */
					sem_post(semVip);
					sem_wait(semVip); /* entrada de um VIP */
					sem_wait(semVisitors); /* quantidade de lugares vazios diminui */
					
					printf("Visitante VIP %d entrou\n", i + 1);
					numVip --; /* diminui o número de VIPs na fila de espera */
					
				} else if(id == SPECIAL_ID) { /* se o visitante for Special */
					sem_post(semSpecial);
					sem_wait(semSpecial); /* entrada de um Special */
					sem_wait(semVisitors); /* quantidade de lugares vazios diminui */
					
					printf("Visitante Special %d entrou\n", i + 1);
					numSpecial--; /* número de Specials na fila de espera diminui */
					
				} else { /* se o visitante for Normal */
					sem_post(semNormal);
					sem_wait(semNormal); /* entrada de um visitante Normal */
					sem_wait(semVisitors); /* quantidade de lugares vazios diminui */

					printf("Visitante Normal %d entrou\n", i + 1);
				} 
				
				sleep(NUM_VISITORS - MAX_VISITORS); /* tempo que os visitantes ficam no cinema */
				
				sem_post(semVisitors); /* quantidade de lugares vagos aumenta */
				printf("Visitante %d saiu\n", i + 1);
				
				if(numVip > 0) { /* VIPs entram em primeiro lugar */
					sem_post(semVip);
				} else if(numVip <= 0 && numSpecial > 0) { /* Specials entram em segundo lugar */
					sem_post(semSpecial);
				} else if(numVip <= 0 && numSpecial <= 0) {
					sem_post(semNormal); /* Visitantes Normal entram em último lugar */
				}
				exit(0);
			}
		}
	}
	
	for(i = 0; i < NUM_VISITORS; i++)
		wait(NULL);
	/* Apaga os semaforos*/
	sem_close(SEM_NAME_1);
	sem_close(SEM_NAME_2);
	sem_close(SEM_NAME_3);
	sem_close(SEM_NAME_4);
	

	/* retira a informacao dos semaforos */
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);
	
	return 0;
}