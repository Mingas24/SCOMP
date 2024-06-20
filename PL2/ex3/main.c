#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    char read_msg1[BUFFER_SIZE];
    char read_msg2[BUFFER_SIZE];
    char write_msg1[BUFFER_SIZE] = "Hello World";
    char write_msg2[BUFFER_SIZE] = "Goodbye!";
    int fd[2];
    pid_t pid;
    int status;

    if (pipe(fd) == -1) {
        perror("Pipe failed!\n");
        return 1;
    }

    pid = fork();

    if (pid > 0) {
        close(fd[0]);

        write(fd[1], write_msg1, sizeof(write_msg1));
        write(fd[1], write_msg2, sizeof(write_msg2));

        close(fd[1]);
        wait(&status);
        if (WIFEXITED(status)) {
            printf("O meu filho %d morreu com %d como valor de saída!\n", pid, WEXITSTATUS(status));
        }
        exit(0);
    }else{
        close(fd[1]);
        read(fd[0], read_msg1, BUFFER_SIZE);
        printf("%s\n", read_msg1);

        read(fd[0], read_msg2, BUFFER_SIZE);
        printf("%s\n", read_msg2);

        close(fd[0]);

        exit(2);
    }
}
