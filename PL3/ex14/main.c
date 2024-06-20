#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
    int circularBuffer[10];
    int positionStatus[10]; // 1 - write-only; 0 - read-only
} Structure;

#define CIRCULAR_BUFFER_ELEMENTS 10

int main() {

    int fd;
    Structure *structAddr;
    //Creates and opens a shared memory space
    fd = shm_open("ex14", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    //Checks if shared memory oppened correctly
    if (fd == -1) {
        printf("Something went wrong when opening shared memory\n");
        exit(1);
    }

    //Truncates the size of the shared memory and initializes it to 0
    ftruncate(fd, sizeof(Structure));

    //Maps a shared memory object in the process address space
    structAddr = mmap(NULL, sizeof(Structure), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    int i;
    // Unlocks all slots of the circular buffer for writing
    for (i = 0; i < CIRCULAR_BUFFER_ELEMENTS; i++)
        structAddr->positionStatus[i] = 1;
    i = 0;


    if (fork() > 0) {
        while (i < 30) {
            // if the next value of the buffer to be altered is available
            if (structAddr->positionStatus[i%CIRCULAR_BUFFER_ELEMENTS] == 1) {
                // sets a new value to that position of the buffer
                structAddr->circularBuffer[i%CIRCULAR_BUFFER_ELEMENTS] = i;
                // sets the status of that position for read-only
                structAddr->positionStatus[i%CIRCULAR_BUFFER_ELEMENTS] = 0;
                i++;
            }
        }

        wait(NULL);

        //Undoes mapping
        if (munmap((void *)structAddr, sizeof(Structure)) < 0) {
            printf("Error at munmap()!\n");
            exit(1);
        }

        //Closes the descriptor
        if (close(fd) < 0) {
            printf("Error at close()!\n");
            exit(1);
        }

        //Removes the file from the system
        if (shm_unlink("ex14") < 0) {
            printf("Error at shm_unlink()!\n");
            exit(1);
        }
    } else {
        while (i < 30) {
            // if the next value of the buffer to be read is available
            if (structAddr->positionStatus[i%CIRCULAR_BUFFER_ELEMENTS] == 0) {
                // prints the value of that position of the buffer
                printf("Value nº%d: %d\n", i+1, structAddr->circularBuffer[i%CIRCULAR_BUFFER_ELEMENTS]);
                // sets the status of that position for write-only
                structAddr->positionStatus[i%CIRCULAR_BUFFER_ELEMENTS] = 1;
                i++;
            }
        }
    }
    return 0;
}