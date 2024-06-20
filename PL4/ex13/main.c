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
#define BUFFER_SIZE 10 /* tamanho do buffer */
#define MAX_NUMBERS 30 /* quantidade máxima de números trocados entre os processos */

#define SEM_NAME_1 "semaforo1_ex13" /* nome do primeiro semáforo */
#define SEM_NAME_2 "semaforo2_ex13" /* nome do segundo semáforo */
#define SEM_NAME_3 "semaforo3_ex13" /* nome do terceiro semáforo */
#define SHM_NAME "/shm_ex13" /* área de memória partilhada */

typedef struct {
	int buffer[BUFFER_SIZE]; /* buffer circular */
	int counter; /* quantidade de números no array */
} memory;

int main() {
	int i, j;
	int dataSize = sizeof(memory);
	
	sem_t *semChild; /* semáforo que controla o acesso do processo filho = produtor */
	sem_t *semFather; /* semáforo que controla o acesso do processo pai = consumidor */
	sem_t *semNumbers; /* semáforo que controla o número de lugares vagos no buffer */
	
	/* cria e abre a memória partilhada devolvendo um descritor de ficheiro */
	int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	/* erro ao criar/abrir área de memória partilhada */
	if(fd == -1){
		perror("Failed shm_open\n");
		return 1;
	}
	
	/* define o tamanho, em bytes, da área de memória partilhada */
	if(ftruncate(fd, dataSize) == -1){
		perror("Failed ftruncate\n");
		return 1;
	}
	
	/* mapeia um objeto de memória partilhada ao espaço de endereçamento do processo */
	memory *memoryPtr = (memory*) mmap(NULL, dataSize, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);
	
	/* erro ao mapear objeto de memória partilhada */
	if(memoryPtr == MAP_FAILED){
		perror("Failed mmap\n");
		return 1;
	}
	
	/* criar semáforos */
	if((semChild = sem_open(SEM_NAME_1, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((semFather = sem_open(SEM_NAME_2, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((semNumbers = sem_open(SEM_NAME_3, O_CREAT | O_EXCL, 0644, BUFFER_SIZE)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	for(i = 0; i < NUM_CHILDS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0){ /* processo filho = produtor */
			while(memoryPtr->counter < MAX_NUMBERS) { /* enquanto houver lugares vagos no buffer */
				
				sem_wait(semNumbers); /* diminuir quantidade de lugares vazios no buffer */
				sem_wait(semChild); /* processo filho avança em 1º lugar e depois bloqueia */
				
				/* introduzir x números no buffer */
				if(memoryPtr->counter < MAX_NUMBERS) { 
					printf("Filho escreveu %d\n", memoryPtr->counter);
					
					/* introduzir valor no buffer */
					memoryPtr->buffer[memoryPtr->counter % BUFFER_SIZE] = memoryPtr->counter;
					
					/* aumentar valor do counter */
					memoryPtr->counter++;
				}
				
				/* se capacidade máxima do buffer for alcançada, o processo pai pode ler */
				if(memoryPtr->counter % BUFFER_SIZE == 0) {
					sem_post(semFather); /* processo pai pode avançar com a leitura dos números */
				}
				
				sem_post(semChild); /* processo filho pode avançar com a escrita */
			}
			exit(0);
		}
	}
	
	/* processo pai = consumidor */
	while(memoryPtr->counter < MAX_NUMBERS) {
				
		sem_wait(semFather); /* processo pai avança com a leitura, mas depois fica bloqueado */
				
		/* processo pai lê números do buffer */
		for(j = 0; j < BUFFER_SIZE; j++) {
			printf("Pai leu %d\n", memoryPtr->buffer[j]);
		}
		
		/* aumentar quantidade de lugares vagos no buffer */
		for(j = 0; j < BUFFER_SIZE; j++) {
			sem_post(semNumbers);
		}
	}
	
	//fechar semaforos
	sem_close(SEM_NAME_1);
	sem_close(SEM_NAME_2);
	sem_close(SEM_NAME_3);

	// remover informacao
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	
	/* desliga um objeto de memória partilhada do espaço de endereçamento do processo */
	if(munmap(memoryPtr, dataSize) == -1){
		perror("Failed munmap\n");
		return 1;
	}
	
	/* fecha o descritor devolvido por shm_open */
	if(close(fd) == -1){
		perror("Failed close\n");
		return 1;
	}
	
	/* remove a memória partilhada do sistema de ficheiros */
	if(shm_unlink(SHM_NAME) == -1) {
		perror("Failed shm_unlink\n");
		return 1;
	}
	
	return 0;
}