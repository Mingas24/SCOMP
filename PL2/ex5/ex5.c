#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

static int LEITURA = 0;
static int ESCRITA = 1;

int main(){
	char mensagem[MAX];
	int Up[2], Down[2];
	
	if(pipe(Up) == -1){
		perror("Erro no pipe Up!\n");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(Down) == -1){
		perror("Erro no pipe Down!\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();
	int status, i;
	if(pid == -1){
		perror("Erro no fork!\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid > 0){
		close(Up[ESCRITA]);
		close(Down[LEITURA]);	
		read(Up[LEITURA], &mensagem, MAX);
		for(i = 0; i < strlen(mensagem) + 1; i++){
			if(mensagem[i] >= 'a' && mensagem[i] <= 'z'){
				mensagem[i] -= 32;
			}else if(mensagem[i] >= 'A' && mensagem[i] <= 'Z'){
				mensagem[i] += 32;
			}		
		}
		write(Down[ESCRITA], &mensagem, strlen(mensagem) + 1);
		wait(&status);
		close(Up[LEITURA]);
		close(Down[ESCRITA]);
	}
	
	if(pid == 0){
		close(Up[LEITURA]);
		close(Down[ESCRITA]);
		printf("Qual é a mensagem?\n");
		fgets(mensagem, MAX, stdin);
		write(Up[ESCRITA], &mensagem, strlen(mensagem)+1);
		read(Down[LEITURA], &mensagem, MAX);
		printf("A mensagem é: %s\n", mensagem);
		close(Up[ESCRITA]);
		close(Down[LEITURA]);
	}
	
	return 0;
}

