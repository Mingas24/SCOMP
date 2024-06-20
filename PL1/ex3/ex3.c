#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
fork(); fork(); fork();
printf("SCOMP!\n");
}


/*
a)  São criados 7 processos

b)

c) São impressas 7 vezes SCOMP.*/
