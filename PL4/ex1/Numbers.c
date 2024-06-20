#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(){
    FILE *file;
    int i;

    file = fopen("Numbers.txt", "w");
    for(i = 0; i < 1600; i++){
        fprintf(file, "%d", i);
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}
