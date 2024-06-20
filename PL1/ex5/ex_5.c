#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

//Pedro Costa 1181481

/*
5. Write a program that creates two child processes, the father waits for each
one of them to terminate and then
outputs their exit value. The first child process created should sleep for 1
second and then exit with a return value
of 1. The second child created should sleep for 2 seconds and exit returning 2.
*/

int main(void){

    int status = 0;

    //Creating two processes
    pid_t ch1 = fork();
    pid_t ch2 = fork();



    //Error when creating a process
    if(ch1 < 0 | ch2 < 0){
        return -1;
    }

    //If we are executing process 1
    if(ch1 == 0){
        sleep(1);
        exit(1);

    }

    //If we are executing process 2
    if(ch2 == 0){
        sleep(2);
        exit(2);
    }

    pid_t aux = waitpid(ch1,&status,0);
    printf("[PID: %d]  %d\n", aux, status);


    pid_t aux2 = waitpid(ch2, &status, 0);
    printf("[PID: %d]  %d\n", aux2, status);

    return 0;
}
