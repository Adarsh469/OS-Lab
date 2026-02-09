#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct shared_data {
    char letter;
    int flag; // 0: Parent turn, 1: Child turn
};

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared_data), 0666 | IPC_CREAT);
    struct shared_data *data = (struct shared_data *)shmat(shmid, NULL, 0);
    data->flag = 0;

    if (fork() == 0) { // Child
        while (data->flag == 0); // Wait for Parent
        printf("Child received: %c\n", data->letter);
        data->letter += 1; // Next alphabet
        data->flag = 0;    // Signal Parent
        shmdt(data);
        exit(0);
    } else { // Parent
        printf("Parent: Enter an English alphabet: ");
        scanf(" %c", &data->letter);
        data->flag = 1; // Signal Child
        
        while (data->flag == 1); // Wait for Child
        printf("Parent received reply: %c\n", data->letter);
        
        wait(NULL);
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}