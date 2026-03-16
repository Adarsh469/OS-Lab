#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/fifo_add1"

int main() {
    mkfifo(FIFO_PATH, 0666);
    printf("FIFO created at: %s\n\n", FIFO_PATH);

    pid_t pid = fork();

    if (pid == 0) {
        int fd = open(FIFO_PATH, O_WRONLY);
        char msg[] = "Hello through a FIFO!";
        printf("Writing: \"%s\"\n", msg);
        write(fd, msg, strlen(msg) + 1);
        close(fd);
        exit(0);
    } else {
        int fd = open(FIFO_PATH, O_RDONLY);
        char buf[100];
        int n = read(fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';
        printf("Reading: \"%s\"\n", buf);
        printf("Bytes read: %d\n", n);
        close(fd);
        wait(NULL);
        unlink(FIFO_PATH);
        printf("FIFO deleted.\n");
    }

    return 0;
}
