#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/fifo_ex2"
#define COUNT 4

int main() {
    mkfifo(FIFO_PATH, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int numbers[COUNT] = {10, 20, 30, 40};
        int fd = open(FIFO_PATH, O_WRONLY);

        printf("Producer writing %d integers into FIFO\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            write(fd, &numbers[i], sizeof(int));
            printf("Producer wrote: %d\n", numbers[i]);
        }

        close(fd);
        exit(0);
    } else {
        int fd = open(FIFO_PATH, O_RDONLY);

        printf("\nConsumer reading %d integers from FIFO\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            int num;
            read(fd, &num, sizeof(int));
            printf("Consumer read: %d\n", num);
        }

        close(fd);
        wait(NULL);
        unlink(FIFO_PATH);
    }

    return 0;
}
