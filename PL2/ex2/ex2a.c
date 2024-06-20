#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void){
	
	int fd[2];
	
	int inteiro_read;
	int inteiro_write;
	
	char string_read [20];
	char string_write [20];
	
	pid_t pid;
	int pipe(fd);
	pid = fork();
	
	if(pid > 0){
		
		close (fd[0]);
		printf ("Insira um número\n");
		scanf ("%d",&inteiro_write);
		printf ("Insira uma palavra\n");
		getchar();
		fgets(string_write,sizeof(string_write),stdin);
		
		write(fd[1],&inteiro_write,sizeof(int));
		write(fd[1],&string_write,strlen(string_write)+1);
		
		close (fd[1]);
		
	}else{ 

		close(fd[1]);
 
		read(fd[0],&inteiro_read,sizeof(int));	
		printf("Número %d \n",inteiro_read); 
	
		read(fd[0],&string_read,strlen(string_read)+1);	
		printf("Palavra: %s\n", string_read);
	
	 	close(fd[0]);	
	}
return 0;
}

