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
#define shm_name "/shm_ex04"



int main(void){
	char char_array[ARRAY_SIZE];
	int sum=0, avg=0;
	
	int fd, data_size= sizeof(char_array);
	char * shared;
	fd=shm_open(shm_name ,O_RDWR, S_IRUSR| S_IWUSR);
	
	ftruncate(fd, data_size);
	shared= (char *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	time_t t; 
	int i; 
	srand ((unsigned) time (&t)); 

	for (i = 0; i < ARRAY_SIZE; i++){ 
		printf("%c; ", *shared);
		sum+=*shared;
		shared+=1;
	}
	avg=sum/ARRAY_SIZE;
	printf("\n		Sum: %d\n		Average: %d\n", sum, avg);
	return 0;
}
