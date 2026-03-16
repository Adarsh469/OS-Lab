#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY   6060
#define LOG_COUNT 6

struct msg {
    long type;
    int  hour;
    int  minute;
    int  user_count;
};

int main() {
    int qid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        struct msg logs[LOG_COUNT] = {
            {1, 9, 0,  5}, {1, 9, 10, 8}, {1, 9, 20, 11},
            {1, 9, 30, 14},{1, 9, 40, 5}, {1, 9, 50, 8}
        };

        printf("Writer: sending %d log entries through message queue\n\n", LOG_COUNT);
        for (int i = 0; i < LOG_COUNT; i++) {
            msgsnd(qid, &logs[i], sizeof(logs[i]) - sizeof(long), 0);
            printf("Writer logged -> Time: %02d:%02d  Users: %d\n",
                   logs[i].hour, logs[i].minute, logs[i].user_count);
        }
        exit(0);
    } else {
        wait(NULL);

        int hourly_total[24] = {0};
        int hourly_count[24] = {0};

        printf("\nReader: reading log entries from message queue\n\n");
        for (int i = 0; i < LOG_COUNT; i++) {
            struct msg e;
            msgrcv(qid, &e, sizeof(e) - sizeof(long), 1, 0);
            printf("Reader got  -> Time: %02d:%02d  Users: %d\n",
                   e.hour, e.minute, e.user_count);
            hourly_total[e.hour] += e.user_count;
            hourly_count[e.hour]++;
        }

        printf("\n--- Hourly Average Users ---\n");
        for (int h = 0; h < 24; h++) {
            if (hourly_count[h] > 0)
                printf("Hour %02d:00  ->  Avg users = %.2f  (%d readings)\n",
                       h, (double)hourly_total[h] / hourly_count[h], hourly_count[h]);
        }

        msgctl(qid, IPC_RMID, 0);
    }

    return 0;
}
