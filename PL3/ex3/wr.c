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
#include <string.h>



typedef struct{
    int num;
    char st[20];
}shared_struct;



int main(void){

    int d; /*descritor para a memória partilhada*/
    int fd[2];
    int i, r;
    pid_t p;
    
    shared_struct array[100000];
    shared_struct rd_array[100000];
    shared_struct *pt;

    shared_struct pp;
    shared_struct rp;
    
    /*tempos em que começa a escrita*/
    clock_t shm_t;
    clock_t pipe_t;

    /*tempo que durou a escrita*/
    clock_t shm_duration;
    clock_t pipe_duration;
    
    /*preencher o array de structs com dados*/
    for(i = 0; i< 100000; i++){
        strcpy(array[i].st, "ISEP - SCOMP 2020");
        array[i].num = 5;
    }


   
    /*#################################
        Shared memory
    ################################*/    

    /*criar e abrir área de memória partilhada*/
    d = shm_open("/ex5", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

     /*verifica se a memória partilhada foi criada com sucesso*/
    if(d == -1){
        printf("### Failed to create shared memory ###");
        return 1;
    }


    /*define tamanho do bloco de memória partilhada*/
    ftruncate(d, sizeof(array));


    /*mapeamento da memória partilhada, na memória do processo*/
    pt = (shared_struct *)mmap(NULL, sizeof(array),PROT_READ|PROT_WRITE, MAP_SHARED, d, 0);


    /*Começar o timer*/
    shm_t = clock();

    /*escreve na memória partilhada*/
    for(i = 0; i < 100000; i++){
       // pt->st[i] = array[i].st;
        strcpy(pt->st,array[i].st);
        pt->num = array[i].num;        

    }

    /*tempo que demorou a escrever*/
    shm_duration = clock() - shm_t;

    /*imprime o tempo que demorou a escrever na memória*/
    printf("tempo de escrita na shm:    %ld ms\n", shm_duration);


    r = munmap(pt, 20);
    if (r < 0) exit(1); /* Check error */

    r = close(d);
    if (r < 0) exit(1); /* Check error*/
    //exit(0);


    /*#################################
        Pipes
    ##################################*/

    /*cria o pipe*/

    if(pipe(fd) == -1){
        printf("### Failed to crete pipe!###\n");
	    return 1;
    }

    p = fork();             /*cria um processo*/


    /*execução do processo pai*/

    if(p > 0){



        /*fecha a extremidade não usada*/
        close (fd[0]);

        /*Começar o timer*/
        shm_t = clock();


        /*escreve no pipe o conteudo do array*/
        for(i = 0; i < 100000; i++){
            pp = array[i];

            write(fd[1], &pp, 24);

        }

        pipe_duration = clock() - pipe_t;   //tempo de escrita em pipe
        
        printf("tempo de escrita no pipe:    %ld ms\n", pipe_duration);


	    /*fecha a extremidade de escrita*/
	    close(fd[1]);


    }

    /*execução do processo filho*/
    else{

        close(fd[1]);
        
        /*Começar o timer*/
        shm_t = clock();
        
        /*lê dados do pipe */
        for(i = 0; i < 100000; i++){
            read(fd[0], &rp, 24);
            rd_array[i]=rp;
        }

        pipe_duration = clock() - pipe_t;   //tempo de leitura em pipe
        
        printf("tempo de leitura no pipe:    %ld ms\n", pipe_duration);

        close(fd[0]);

    }
    
    


}