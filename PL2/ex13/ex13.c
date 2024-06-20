#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	int f[5][2], i, nproduct = 1000, inventory = 0, buffer = 0, piece, one = 1;
	char sms[4][150] = {"Machine M1 cut 5 pieces. Transfering pieces to M2.",
		"Machine M2 folded pieces. Transfering pieces to M3.",
		"Machine M3 welded 10 pieces. Transfering pieces to M4.",
		"Machine M4 packed 100 pieces. Transfering pieces to storage A1."};
	pid_t p;
	size_t snum = sizeof(int);
	for(i = 0; i < 5; i++){
		if(pipe(f[i]) == -1){
			perror("Failed");
			return 1;
		}
	}
	//Maquina 1
	p = fork();
	if(p == 0){
		close(f[0][1]);
		close(f[1][0]);
		while(nproduct > 0){
			if(read(f[0][0], &piece, snum) > 0) {
				buffer+= piece;
				nproduct-= piece;
				if(buffer == 5) {
					//Tempo de espera para tornar mais realista
					sleep(2);
					printf("\n%s\n", sms[0]);
					for(i = 0; i < 5; i++){
						write(f[1][1], &one, snum);
					}
					buffer = 0;
				}
			}
		}
		close(f[0][0]);
		close(f[1][1]);
		exit(0);
	}
	//Maquina 2
	p = fork();
	if(p == 0){
		close(f[1][1]);
		close(f[2][0]);
		while(nproduct > 0){
			if(read(f[1][0], &piece, snum) > 0) {
				nproduct-= piece;
				//Tempo de espera para tornar mais realista
				sleep(2);
				printf("\n%s\n", sms[1]);
				write(f[2][1], &piece, snum);
			}
		}
		close(f[1][0]);
		close(f[2][1]);
		exit(0);
	}
	//Maquina 3
	p = fork();
	if(p == 0){
		close(f[2][1]);
		close(f[3][0]);
		while(nproduct > 0){
			if(read(f[2][0], &piece, snum) > 0) {
				buffer+= piece;
				nproduct-= piece;
				if(buffer == 10) {
					//Tempo de espera para tornar mais realista
					sleep(2);
					printf("\n%s\n", sms[2]);
					buffer = 1;
					for(i = 0; i < 10; i++){
						write(f[3][1], &buffer, snum);
					}
					buffer = 0;
				}
			}
		}
		close(f[2][0]);
		close(f[3][1]);
		exit(0);
	}
	//Maquina 4
	p = fork();
	if(p == 0){
		close(f[3][1]);
		close(f[4][0]);
		while(nproduct > 0){
			if(read(f[3][0], &piece, snum) > 0) {
				buffer+= piece;
				nproduct-= piece;
				if(buffer == 100) {
					printf("\n%s\n", sms[3]);
					buffer = 1;
					for(i = 0; i < 100; i++){
						write(f[4][1], &buffer, snum);
					}
					buffer = 0;
				}
			}
		}
		close(f[3][0]);
		close(f[4][1]);
		exit(0);
	}
	close(f[0][0]);
	for(i = 0; i < nproduct; i++) {
		write(f[0][1], &one, snum);
	}
	close(f[0][1]);
	close(f[4][1]);
	while(inventory < nproduct) {
		read(f[4][0], &buffer, snum);
		inventory+=buffer;
	}
	close(f[4][0]);	
	printf("Total produzido: %d\n", inventory);
	return 0;
}