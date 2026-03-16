#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define COUNT 4

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);

        int numbers[COUNT] = {10, 20, 30, 40};
        printf("Producer writing %d integers into pipe\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            write(fd[1], &numbers[i], sizeof(int));
            printf("Producer wrote: %d\n", numbers[i]);
        }

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        wait(NULL);

        printf("\nConsumer reading %d integers from pipe\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            int num;
            read(fd[0], &num, sizeof(int));
            printf("Consumer read: %d\n", num);
        }

        close(fd[0]);
    }

    return 0;
}
