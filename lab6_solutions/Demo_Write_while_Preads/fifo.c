#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/fifo_add2"

int main() {
    mkfifo(FIFO_PATH, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int fd = open(FIFO_PATH, O_WRONLY);
        char msg[] = "Message from child process through FIFO!";
        printf("Child (PID %d): writing -> \"%s\"\n", getpid(), msg);
        write(fd, msg, strlen(msg) + 1);
        close(fd);
        exit(0);
    } else {
        int fd = open(FIFO_PATH, O_RDONLY);
        char buf[256];
        int n = read(fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';
        printf("Parent (PID %d): read   -> \"%s\"\n", getpid(), buf);
        printf("Parent: bytes received = %d\n", n);
        close(fd);
        wait(NULL);
        unlink(FIFO_PATH);
    }

    return 0;
}
