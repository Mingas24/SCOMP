#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {               
       int  int_Escrita;
       char string_Escrita[20];     
     
} s1;

int main (void) {
 
     s1 structRead;

     int int_Input;
	 
     char string_read[20];     
     int  int_read;

int fd[2];
   

if(pipe(fd)==-1){
	perror("Pipe failed");
	return 1;
}

pid_t p= fork();


if (p>0){

	close (fd[0]);

printf("Inteiro:\n"); 
scanf ("%d",&int_Input);

printf("String:\n"); 
		getchar();
		fgets(structRead.string_Escrita,sizeof(structRead.string_Escrita),stdin);

		structRead.int_Escrita=int_Input;
		write(fd[1],&structRead,24);

		close(fd[1]);
		
}else{
	
close(fd[1]);

 read(fd[0],&int_read,4);
  printf("Li o número da estrutura  %d \n",int_read); 
  
 read(fd[0],&string_read,20);	
 printf("Li a paravra da estrutura: %s\n", string_read);
	
close(fd[0]);

}

return 0;

}
