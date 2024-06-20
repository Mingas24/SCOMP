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

#define CAPAC_MAX 200
#define NR_PASSAGEIROS 250

typedef struct {
	int passageiros;
} shared_data;

void sairComboio(shared_data* comboio, int idProcesso){
	comboio->passageiros--;
	printf("Processo %d : Saí do comboio.\n", idProcesso);
}

void entrarComboio(shared_data* comboio, int idProcesso){
	comboio->passageiros++;
	printf("Processo %d : Entrei no comboio.\n", idProcesso);
}

int main(int argc, char* argv[]){
	shared_data* comboio;
	int fd, data_size = sizeof(shared_data), i, podeEntrar;
		pid_t filhos[NR_PASSAGEIROS];

	/* Cria a memória partilhada*/
	fd = shm_open("/shm_ex11", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
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
	comboio = (shared_data*)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	/* Cria o semáforo com o valor inicial a 1*/
	sem_t* semaforo = sem_open("/sem_geral", O_CREAT, 0644, 1);

	/* Cria o semáforo com o valor inicial a 1*/
	sem_t* sem_porta1 = sem_open("/sem_porta1", O_CREAT, 0644, 1);

	/* Cria o semáforo com o valor inicial a 1*/
	sem_t* sem_porta2 = sem_open("/sem_porta2", O_CREAT, 0644, 1);

	/* Cria o semáforo com o valor inicial a 1*/
	sem_t* sem_porta3 = sem_open("/sem_porta3", O_CREAT, 0644, 1);

	if(sem_porta1 == SEM_FAILED || sem_porta2 == SEM_FAILED || sem_porta3 == SEM_FAILED || semaforo == SEM_FAILED){
			perror("Erro na criação dos semáforos.\n");
			exit(1);
	}

	/* Inicializada o nr de passageiros do comboio a 0*/
	comboio->passageiros = 0;
		for(i = 0; i < NR_PASSAGEIROS; i++){
		filhos[i] = fork();
		if(filhos[i] == -1){
			perror("Erro na criação do filho\n");
			exit(1);
		}
		if(filhos[i] == 0){
			podeEntrar = 0;
			while(!podeEntrar){
				sem_wait(semaforo);
				/* Verifica se o comboio está cheio*/
				if(comboio->passageiros == CAPAC_MAX){
					sem_post(semaforo);
					printf("Comboio cheio.\n");
					sleep(2);
				}else{
					podeEntrar = 1;
				}
			}
			podeEntrar = 0;
			while(!podeEntrar){
				if(sem_trywait(sem_porta1) && podeEntrar == 0){
					entrarComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta1);
				} 
				if(sem_trywait(sem_porta2) && podeEntrar == 0){
					entrarComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta2);
				} 
				if(sem_trywait(sem_porta3)&& podeEntrar == 0){
					entrarComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta3);
				}
			}
			 sem_post(semaforo);
			     sleep(1);
			sem_wait(semaforo);
			podeEntrar=0;
			while(!podeEntrar){
				if(sem_trywait(sem_porta1)&& podeEntrar == 0){
					sairComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta1);
				}
				if(sem_trywait(sem_porta2)&& podeEntrar == 0){
					sairComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta2);
				}
				if(sem_trywait(sem_porta3)&& podeEntrar == 0){
					sairComboio(comboio, i+1);
					podeEntrar = 1;
					sem_post(sem_porta3);
				}
			}
			sem_post(semaforo);

        exit(0);
		}
	}

	/* Espera que todos os filhos terminem. */
	for(i = 0; i < NR_PASSAGEIROS; i++){
		wait(NULL);
	}

	sem_unlink("/sem_porta1");
	sem_unlink("/sem_porta2");
	sem_unlink("/sem_porta3");
	sem_unlink("/sem_geral");

	/* Disconecta a área de memória partilhada do espaço de memória reservado para ela*/
	if(munmap(comboio, data_size) == -1){
		perror("Erro no munmap.\n");
		exit(1);
	}

	/* Remove a área de memória partilhada*/
	if(shm_unlink("/shm_ex11") == -1){
		perror("Erro a remover a área de memória partilhada.\n");
		exit(1);
	}
	return 0;
}
