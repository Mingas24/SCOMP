#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /*necessário para a utilização do fork*/
#include <stdlib.h> /*necessário para a utilização do exit*/
#include <sys/wait.h> /*necessário para o wait */
#include <fcntl.h> /* opções de controlo do ficheiro */
#include <sys/types.h> /* tipos de data usados pela API*/
#include <sys/stat.h> /* constantes usadas para abrir*/
#include <sys/mman.h> /* declarações relacionadas com a gestão da memória partilhada */
#include <semaphore.h>
#include <time.h>

#define CAPAC_MAX 5
#define NR_VISITANTES 20
#define INICIO 1
#define FIM 20

typedef struct {
    int visitantes;
} shared_data;

void sairSala(shared_data* sala, int idProcesso){
    sala->visitantes--;
    printf("Processo %d : Saí da sala.\n", idProcesso);
}

void entrarSala(shared_data* sala, int idProcesso){
    sala->visitantes++;
    printf("Processo %d : Entrei na sala.\n", idProcesso);
}

int main(){
    shared_data* sala;
    int fd, data_size = sizeof(shared_data), i, podeEntrar;
    time_t agora, contTempo;
    pid_t filhos[NR_VISITANTES];

    /* Cria a memória partilhada*/
    fd = shm_open("/shm_ex15", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd == -1){
        perror("Erro na criação da memória partilhada.\n");
        exit(1);
    }

    /* Define o tamanho da área de memória partilhada */
    if(ftruncate(fd, data_size) == -1){
        perror("Erro no truncate.\n");
        exit(1);
    }

    /* Coloca a área de memória partilhada num endereço à escolha do sistema e
    * devolve um apontador para o objeto */
    sala = (shared_data*)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    /* Cria um semáforo para cada porta e um semáforo geral todos com o valor inicial a 1*/
    sem_t* semaforo = sem_open("/sem_geral", O_CREAT, 0644, 1);

    if(semaforo == SEM_FAILED){
        perror("Erro na criação do semáforo.\n");
        exit(1);
    }

    sala->visitantes = 0;
    for(i = 0; i < NR_VISITANTES; i++){
        filhos[i] = fork();
        contTempo = time(NULL);
        if(filhos[i] == -1){
            perror("Erro na criação do filho\n");
            exit(1);
        }
        if(filhos[i] == 0){
            podeEntrar = 0;
            while(podeEntrar == 0){
                sem_wait(semaforo);
                /* Verifica se a sala está cheia*/
                if(sala->visitantes == CAPAC_MAX){
                    printf("Processo %d : Sala cheia.\n", i+1);
                    sem_post(semaforo);
                    sleep(2);
                }else{
                    podeEntrar = 1;
                }
            }
            agora = time(NULL);
            while(podeEntrar == 1){
                if((agora-contTempo) < INICIO || (agora-contTempo) > FIM ){
                    sleep(1);
                    printf("Processo %d : Ainda não posso entrar na sala.\n", i+1);
                    agora = time(NULL);
                }
                if(podeEntrar == 1 && (agora-contTempo) >= INICIO  && (agora-contTempo) <= FIM ){
                    entrarSala(sala, i+1);
                    podeEntrar = 0;
                }
            }
            sem_post(semaforo);
            sleep(1);
            sem_wait(semaforo);
            agora = time(NULL);
            while(podeEntrar == 0){
                if((agora-contTempo) > FIM || (agora-contTempo) < INICIO){
                    sleep(1);
                    printf("Processo %d : Ainda não posso sair da sala.\n", i+1);
                    agora = time(NULL);
                }
                if(podeEntrar == 0 && (agora-contTempo) <= FIM && (agora-contTempo) >= INICIO){
                    sairSala(sala, i+1);
                    podeEntrar = 1;
                    sem_post(semaforo);
                    exit(0);
                }
            }
        }
    }

    /* Espera que todos os filhos terminem */
    for(i = 0; i < NR_VISITANTES; i++){
        wait(NULL);
    }

    /* Desconecta os semáforos*/
    sem_unlink("/sem_geral");

    /* Disconecta a área de memória partilhada do espaço de memória reservado para ela*/
    if(munmap(sala, data_size) == -1){
        perror("Erro no munmap.\n");
        exit(1);
    }

    /* Remove a área de memória partilhada*/
    if(shm_unlink("/shm_ex15") == -1){
        perror("Erro a remover a área de memória partilhada.\n");
        exit(1);
    }
    return 0;
}
