#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int main(void){
	
int fd1[2];
int fd2[2];

int int_read;
   
/*Pai - filho*/
if(pipe(fd1)==-1){
	perror("Pipe failed");
	return 1;
}
/*Filho - pai*/
if(pipe(fd2)==-1){
	perror("Pipe failed");
	return 1;
}
pid_t pid= fork();

if(pid>0){
	
int creditoChild=20;	

 int n=0; 
 time_t t;
  srand ((unsigned) time (&t));
  n = rand () % 5; //Como gera números de 0-4 temos de adicionar 1 de modo que seja de 1-5
  n++;
  int aceita = 1;
  int recusa = 0;
  
            // se o crédito for positivo , escreve 1 e aceita aposta
                 if(creditoChild>0){

                 /*escreve no pipe */
	              write(fd1[1],&aceita,sizeof(int));
	              
	               /*lê dados do pipe */
                  read(fd2[0],&int_read,4);   
                           
                       if(int_read == n){
						   creditoChild+=10;
	
					   }else{ 
	                         creditoChild-=5;
	                   }
	             
			  }else{   // se o crédito for negativo ou 0 , escreve 0
                 /*escreve no pipe */
	              write(fd1[1],&recusa,sizeof(int));               
 } 
 
 write(fd1[1],&creditoChild,sizeof(int));
 
 close(fd1[0]);
 close(fd1[1]);
 close(fd2[0]);
 close(fd2[1]);
 
 }else { //child

 /*lê dados do pipe */

 read(fd1[0],&int_read,4);    
  
     if(int_read==1){       
		 
	//Pode apostar:
		int n=0;
		time_t t1;
                     srand ((unsigned) time (&t1));
                     
                     //temos de atribuir tempos diferentes ao srand para que os números do pai e do filho não sejam sequencialmente os mesmos
                     
                      n = rand () % 6;
                      n++;
                      if(n==6){ 
                      n = rand() % 5;// Como gera números de 0-4 temos de adicionar 1 de modo que seja de 1-5
                      n++;
					  }
                      
                      /*escreve aposta no pipe */
	              write(fd2[1],&n,sizeof(int));

				  }else{		  
					 printf("Aposta cancelada porque não tem crédito");      
                      }
       read(fd1[0],&int_read,4);  
       
       printf("Tem crédito de %d  \n",int_read);
                       
close(fd1[0]);
close(fd1[1]);
close(fd2[0]);
close(fd2[1]);             
     }                 
    return 0;
  }
