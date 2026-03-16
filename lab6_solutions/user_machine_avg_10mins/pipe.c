#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define LOG_COUNT 6

struct log_entry {
    int hour;
    int minute;
    int user_count;
};

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);

        struct log_entry logs[LOG_COUNT] = {
            {9, 0,  5}, {9, 10, 8}, {9, 20, 11},
            {9, 30, 14},{9, 40, 5}, {9, 50, 8}
        };

        printf("Writer: sending %d log entries through pipe\n\n", LOG_COUNT);
        for (int i = 0; i < LOG_COUNT; i++) {
            write(fd[1], &logs[i], sizeof(struct log_entry));
            printf("Writer logged -> Time: %02d:%02d  Users: %d\n",
                   logs[i].hour, logs[i].minute, logs[i].user_count);
        }

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        wait(NULL);

        int hourly_total[24] = {0};
        int hourly_count[24] = {0};

        printf("\nReader: reading log entries from pipe\n\n");
        for (int i = 0; i < LOG_COUNT; i++) {
            struct log_entry e;
            read(fd[0], &e, sizeof(struct log_entry));
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

        close(fd[0]);
    }

    return 0;
}
