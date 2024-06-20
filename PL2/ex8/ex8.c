#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/*
Write a program that creates 10 child processes to play the game “Win the pipe!”. 
There must be only one pipe, shared by all processes. 
The game’s rules are as follow:

a. The parent process fills, each 2 seconds, a structure with a message “Win” 
and the round’s number (1 to 10) and writes this data in the pipe;

b. Each of child processes is trying to read data from the pipe. The child that 
succeeds should print the winning message and the round’s number, 
ending its execution with an exit value equal to the winning round’s number;

c.
The remaining child processes continue to try to read data from the pipe;

d. After all child processes terminate, the parent process should print the 
PID and the winning round of each child.*/




/*struct que contém a mensagem e o número da ronda*/
typedef struct{
	char message[10];
	int round;
}game;


int main() {


	/*array com os descritores para o pipe*/
	int fd[2];


	pid_t p;
	
	game fst_game;
	strcpy(fst_game.message,"Win!!!");
	
	
	int i;
    int x;
    int winning_round[10];
    int child_pid[10];
    
	

	/*verifica se o pipe foi criado com sucesso*/
	if(pipe(fd) == -1){
		perror("### Failed to create pipe! ###\n");
		return -1;
	}


	/*criaçao de 10 processos filho*/
	for(i = 0; i < 10; i++){
		p = fork();

		/*verifica se o processo foi criado com sucesso*/
		if(p == -1){
			printf("### Failed to create child process ###\n");
			return -1;
		}

		/*execução do processo pai*/
		if(p == 0){

			/*fecha a extremidade não usada, write*/
			close(fd[1]);

            /*se conseguiu ler alguma coisa do pipe*/
			if(read(fd[0], &fst_game, sizeof(game)) != 0){

                /*imprime a mensagem de win e o número da ronda*/
				printf("%s\n Round number: %d\n",fst_game.message, fst_game.round);

                /*dá exit com o número da ronda em que ganhou*/
				exit(i+1);
			}

			/*fecha a extremidade de read*/
			close(fd[0]);
		}
	}
	

	for(i=0; i<10; i++){
		
		fst_game.round = i+1;
		
        /*fecha a extremidade de read do pipe*/
        close(fd[0]);

        /*escreve no pipe*/
		write(fd[1], &fst_game, sizeof(game));

        
		child_pid[i] = wait(&x);
		winning_round[i] = WEXITSTATUS(x);
        
	}
    /*fecha a extremidade de write do pipe*/
    close(fd[1]);
	
	

    /*imprime o pid e o número da ronda, em que cada processo filho ganhou*/
	for(i=0; i<10; i++){
		printf("Round number: %d\n	PID:%i\n", winning_round[i], child_pid[i]);
	}
}
