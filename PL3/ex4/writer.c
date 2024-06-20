#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 	

#define ARRAY_SIZE 100
#define shm_name "/shm_ex08"


int main(void){
	
	char char_array[ARRAY_SIZE], char_random;
	
	int fd, data_size= sizeof(char_array);
	char *shared;
	fd=shm_open(shm_name , O_CREAT | O_RDWR, S_IRUSR| S_IWUSR);
	
	ftruncate(fd, data_size);
	shared= (char *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	time_t t; /* para iniciar os numeros aleatorios */ 
	int i; 
	/* inicia o randomizer */ 
	srand ((unsigned) time (&t)); 

	/* inicializa o array com numeros aleatorios (rand(): stdlib.h) */ 
	for (i = 0; i < ARRAY_SIZE; i++){ 
		char_random = rand () % 90; 
		while(char_random<65){
			char_random = rand () % 90; 
		}
		*shared=char_random;
		shared+=1;
	}
	
	return 0;
}
