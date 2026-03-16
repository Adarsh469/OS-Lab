#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 3333

struct shared {
    int  number;
    char result[30];
    int  number_ready;
    int  result_ready;
};

int is_palindrome(int n) {
    if (n < 0) return 0;
    int original = n, reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return original == reversed;
}

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);

    shm->number_ready = 0;
    shm->result_ready  = 0;

    pid_t pid = fork();

    if (pid == 0) {
        while (shm->number_ready == 0);

        int num = shm->number;
        printf("Process B received: %d\n", num);

        if (is_palindrome(num))
            strcpy(shm->result, "Palindrome");
        else
            strcpy(shm->result, "Not a Palindrome");

        printf("Process B sending result: %s\n", shm->result);
        shm->result_ready = 1;

        shmdt(shm);
        exit(0);
    } else {
        int num;
        printf("Process A - Enter a number: ");
        scanf("%d", &num);

        shm->number       = num;
        shm->number_ready = 1;
        printf("Process A sent: %d\n", num);

        while (shm->result_ready == 0);
        printf("Process A received result: %d is %s\n", num, shm->result);

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
