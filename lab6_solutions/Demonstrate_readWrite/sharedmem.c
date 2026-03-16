#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 3030

int main() {
    int shmid = shmget((key_t)SHM_KEY, 100, 0666 | IPC_CREAT);
    printf("Shared memory created with id: %d\n\n", shmid);

    char *shm = (char *)shmat(shmid, NULL, 0);

    char msg[] = "Hello through shared memory!";
    printf("Writing: \"%s\"\n", msg);
    strcpy(shm, msg);

    char buf[100];
    strcpy(buf, shm);
    printf("Reading: \"%s\"\n", buf);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);
    printf("Shared memory detached and deleted.\n");
    return 0;
}
