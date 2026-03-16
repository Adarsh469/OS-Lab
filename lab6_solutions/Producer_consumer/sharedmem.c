#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 5555
#define COUNT   4

struct shared {
    int numbers[COUNT];
    int ready;
};

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);
    shm->ready = 0;

    pid_t pid = fork();

    if (pid == 0) {
        int numbers[COUNT] = {10, 20, 30, 40};
        printf("Producer writing %d integers into shared memory\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            shm->numbers[i] = numbers[i];
            printf("Producer wrote: %d\n", numbers[i]);
        }
        shm->ready = 1;

        shmdt(shm);
        exit(0);
    } else {
        while (shm->ready == 0);

        printf("\nConsumer reading %d integers from shared memory\n", COUNT);
        for (int i = 0; i < COUNT; i++)
            printf("Consumer read: %d\n", shm->numbers[i]);

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
