#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY   7070
#define LOG_COUNT 6

struct log_entry {
    int hour;
    int minute;
    int user_count;
};

struct shared {
    struct log_entry logs[LOG_COUNT];
    int count;
    int ready;
};

int main() {
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared *shm = (struct shared *)shmat(shmid, NULL, 0);
    shm->ready = 0;
    shm->count = LOG_COUNT;

    pid_t pid = fork();

    if (pid == 0) {
        struct log_entry data[LOG_COUNT] = {
            {9, 0,  5}, {9, 10, 8}, {9, 20, 11},
            {9, 30, 14},{9, 40, 5}, {9, 50, 8}
        };

        printf("Writer: writing %d log entries into shared memory\n\n", LOG_COUNT);
        for (int i = 0; i < LOG_COUNT; i++) {
            shm->logs[i] = data[i];
            printf("Writer logged -> Time: %02d:%02d  Users: %d\n",
                   data[i].hour, data[i].minute, data[i].user_count);
        }
        shm->ready = 1;

        shmdt(shm);
        exit(0);
    } else {
        while (shm->ready == 0);

        int hourly_total[24] = {0};
        int hourly_count[24] = {0};

        printf("\nReader: reading log entries from shared memory\n\n");
        for (int i = 0; i < shm->count; i++) {
            printf("Reader got  -> Time: %02d:%02d  Users: %d\n",
                   shm->logs[i].hour, shm->logs[i].minute, shm->logs[i].user_count);
            hourly_total[shm->logs[i].hour] += shm->logs[i].user_count;
            hourly_count[shm->logs[i].hour]++;
        }

        printf("\n--- Hourly Average Users ---\n");
        for (int h = 0; h < 24; h++) {
            if (hourly_count[h] > 0)
                printf("Hour %02d:00  ->  Avg users = %.2f  (%d readings)\n",
                       h, (double)hourly_total[h] / hourly_count[h], hourly_count[h]);
        }

        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
