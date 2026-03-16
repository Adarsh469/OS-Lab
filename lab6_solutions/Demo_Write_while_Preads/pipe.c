#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);

        char msg[] = "Message from child process through the pipe!";
        printf("Child (PID %d): writing -> \"%s\"\n", getpid(), msg);
        write(fd[1], msg, strlen(msg) + 1);

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        wait(NULL);

        char buf[256];
        int n = read(fd[0], buf, sizeof(buf) - 1);
        buf[n] = '\0';
        printf("Parent (PID %d): read   -> \"%s\"\n", getpid(), buf);
        printf("Parent: bytes received = %d\n", n);

        close(fd[0]);
    }

    return 0;
}
