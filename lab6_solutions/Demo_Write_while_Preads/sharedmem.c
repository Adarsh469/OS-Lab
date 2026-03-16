#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 5050

struct shared {
    char text[256];
    int  ready;
};

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);
    shm->ready = 0;

    pid_t pid = fork();

    if (pid == 0) {
        strcpy(shm->text, "Message from child process through shared memory!");
        printf("Child (PID %d): writing -> \"%s\"\n", getpid(), shm->text);
        shm->ready = 1;

        shmdt(shm);
        exit(0);
    } else {
        while (shm->ready == 0);

        printf("Parent (PID %d): read   -> \"%s\"\n", getpid(), shm->text);

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
