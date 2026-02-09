#include <stdio.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    key_t key = ftok("shm_words", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char*) shmat(shmid, (void*)0, 0);

    printf("Producer: Enter words to write: ");
    fgets(str, 1024, stdin);

    printf("Data written to memory: %s", str);
    shmdt(str);
    return 0;
}