#include <stdio.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shm_words", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char*) shmat(shmid, (void*)0, 0);

    printf("Consumer read words: %s", str);

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL); // Delete after use
    printf("Shared memory detached and deleted.\n");
    return 0;
}