#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 8888

struct shared {
    char letter;
    char reply;
    int  sent_by_parent;
    int  sent_by_child;
};

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);
    shm->sent_by_parent = 0;
    shm->sent_by_child  = 0;

    pid_t pid = fork();

    if (pid == 0) {
        while (shm->sent_by_parent == 0);

        printf("Child received: %c\n", shm->letter);
        shm->reply = shm->letter + 1;
        printf("Child sending: %c\n", shm->reply);
        shm->sent_by_child = 1;

        shmdt(shm);
        exit(0);
    } else {
        char letter;
        printf("Parent - Enter an alphabet: ");
        scanf(" %c", &letter);

        shm->letter         = letter;
        shm->sent_by_parent = 1;
        printf("Parent sent: %c\n", letter);

        while (shm->sent_by_child == 0);
        printf("Parent received reply from child: %c\n", shm->reply);

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
