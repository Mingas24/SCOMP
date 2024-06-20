#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define FILHOS 10
#define SIZE 50000

static int LEITURA = 0;
static int ESCRITA = 1;

typedef struct{
	int customer_code;
	int product_code;
	int quantity;
}Sales;


int main(){
	int fd[2], produtos[SIZE];
	Sales sales[SIZE];
	pid_t pid;
	int i, j = 0, status;
	int	k = 0;
	
	srand((unsigned) time(NULL));
	for(i = 0; i < SIZE; i++){
		sales[i].customer_code = rand() % 10;
		sales[i].product_code = i;
		sales[i].quantity = rand() % 25;
	}
	
	if(pipe(fd) == -1){
		perror("Erro no pipe!\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < FILHOS; i++){
		pid = fork();
		if(pid == -1){
			perror("Erro no fork!\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			close(fd[LEITURA]);
			for(j = (SIZE/FILHOS) * i; j < (SIZE/FILHOS) * (i+1); j++){
				if(sales[j].quantity > 20){
					write(fd[ESCRITA], &(sales[j].product_code), sizeof(sales[j].product_code));
				}
			}
			close(fd[ESCRITA]);
			exit(0);
		}
	}	
	
		close(fd[ESCRITA]);
		while(read(fd[LEITURA],&(sales[j].product_code),sizeof(sales[j].product_code)) > 0){
			produtos[k] = sales[j].product_code;
			k++;
		}
		close(fd[LEITURA]);
		for(i = 0; i < FILHOS; i++){
			wait(&status);
		}
		for(i = 0; i < k; i++){
			printf("Codigo:%d	", produtos[i]);
		}
		printf("\nSão os produtos vendindos mais que 20 vezes.\n");

	return 0;
}

