#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


#define BUFFER 80

/*
4. Write a program that creates a child process and establishes with 
it a communicating channel through a pipe. The parent process should 
send the contents of a text file to its child through the pipe. 
The child should print all the received data. 
The parent must wait for its child to end.*/


int main(){

    
    int fd[2];              /*array para os descritores do pipe*/
    char s_read[BUFFER];       /*string que irá conter o conteúdo lido do ficheiro*/

    char f_string[BUFFER];

    int i = 0;
    FILE *fp;                /*apontador para um ficheiro*/

    pid_t p;


    /*verifica se o pipe foi criado com sucesso*/
    if(pipe(fd) == -1){
        perror("### Failed to create pipe! ###\n");
        return 1;
    }




    /*criação do processo filho*/
    p = fork();

    


    /*execução do processo pai*/
    if(p > 0){

        /*fecha a estremidade read*/
        close(fd[0]);

        /*abrir o ficheiro de texto*/
        fp = fopen("ex4.txt", "r");


        if( fgets (f_string, BUFFER, fp)!=NULL ) {
        
        puts(f_string);
        }
            fclose(fp);






       

        /*escreve o conteúdo do ficheiro de texto no pipe*/
        write(fd[1], f_string, strlen(f_string));

        /*fecha a extremidade de write*/
        close(fd[1]);

        /*fecha o ficheiro*/
        fclose(fp);

    }


    /*execução do processo filho*/
    else{

    /*fecha a extremidade de write no pipe*/
    close(fd[1]);

    read(fd[0], s_read, BUFFER);

    /*fecha a extremidade read*/
    close(fd[0]);

    /*fecha o ficheiro*/
    fclose(fp);

    printf("%s\n", s_read); 
    }

    /*processo pai aguarda que o processo filho termine*/


 return 0;

}