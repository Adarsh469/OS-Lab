#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/fifo_add3"
#define LOG_COUNT 6

struct log_entry {
    int hour;
    int minute;
    int user_count;
};

int main() {
    mkfifo(FIFO_PATH, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int fd = open(FIFO_PATH, O_WRONLY);

        struct log_entry data[LOG_COUNT] = {
            {9, 0,  5}, {9, 10, 8}, {9, 20, 11},
            {9, 30, 14},{9, 40, 5}, {9, 50, 8}
        };

        printf("Writer: sending %d log entries through FIFO\n\n", LOG_COUNT);
        for (int i = 0; i < LOG_COUNT; i++) {
            write(fd, &data[i], sizeof(struct log_entry));
            printf("Writer logged -> Time: %02d:%02d  Users: %d\n",
                   data[i].hour, data[i].minute, data[i].user_count);
        }

        close(fd);
        exit(0);
    } else {
        int fd = open(FIFO_PATH, O_RDONLY);

        int hourly_total[24] = {0};
        int hourly_count[24] = {0};

        printf("\nReader: reading log entries from FIFO\n\n");
        for (int i = 0; i < LOG_COUNT; i++) {
            struct log_entry e;
            read(fd, &e, sizeof(struct log_entry));
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

        close(fd);
        wait(NULL);
        unlink(FIFO_PATH);
    }

    return 0;
}
