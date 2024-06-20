#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

static int n_filhos = 8;
static int n_sems = 2;

int main()
{
    sem_t *sem[n_sems];
    char sems_names[][25] = {"/semaforo_input_ex01", "/semaforo_output_ex01"};
    int sems_init_vales[] = {1, 1};
    int line_Number = 0, number[1600], status, j, out;
    pid_t pid;
    FILE *f_in, *f_out;
    enum index
    {
        INPUT,
        OUTPUT
    };

    int i;
    for (i = 0; i < n_sems; i++)
    {
        if ((sem[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, sems_init_vales[i])) == SEM_FAILED)
        {
            perror("No sem_open()");
            exit(1);
        }
    }

	for(i = 0; i < n_filhos; i++){
		pid = fork();
		if (pid == -1){
			perror("Erro no fork!\n");
			exit(EXIT_FAILURE);
		} 
		
		if (pid == 0){
			sem_wait(sem[INPUT]);
			f_in = fopen("numbers.txt", "r");
			for(j = i*200; j < (i+1)*200; j++){
				while(line_Number != i*200){
					fscanf(f_in, "%d", &number[line_Number]);
					line_Number++;
				}
				fscanf(f_in, "%d", &number[j]);
			}
			fclose(f_in);
			
			sem_post(sem[INPUT]);
			sem_wait(sem[OUTPUT]);
			f_out = fopen("output.txt", "a");
			for(j = i*200; j < (i+1)*200; j++){
				fprintf(f_out, "%d\n", number[j]);
			}
			fclose(f_out);
			sem_post(sem[OUTPUT]);
			exit(0);
		}
	}
	
	while(wait(&status) > 0);
	
	f_out = fopen("output.txt", "r");
	while (fscanf(f_out, "%d", &out) != EOF){
		printf("%d\n", out);
	}
	fclose(f_out);
	sem_unlink(sems_names[INPUT]);
	sem_unlink(sems_names[OUTPUT]);
    
    return 0;
}
