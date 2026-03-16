#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY   1010
#define MAX_WORDS 5
#define WORD_LEN  50

struct shared {
    char words[MAX_WORDS][WORD_LEN];
    int  count;
    int  ready;
};

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);
    shm->ready = 0;
    shm->count = 0;

    pid_t pid = fork();

    if (pid == 0) {
        int n;
        printf("Producer - How many words (max %d): ", MAX_WORDS);
        scanf("%d", &n);
        if (n > MAX_WORDS) n = MAX_WORDS;

        printf("Producer - Enter %d words:\n", n);
        for (int i = 0; i < n; i++) {
            printf("  Word %d: ", i + 1);
            scanf("%s", shm->words[i]);
            printf("Producer wrote: %s\n", shm->words[i]);
        }
        shm->count = n;
        shm->ready = 1;

        shmdt(shm);
        exit(0);
    } else {
        while (shm->ready == 0);

        printf("\nConsumer reading %d words from shared memory\n", shm->count);
        for (int i = 0; i < shm->count; i++)
            printf("Consumer read: %s\n", shm->words[i]);

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
        printf("Shared memory detached and deleted.\n");
    }

    return 0;
}
