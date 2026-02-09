#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("[Child] Replacing image with Exercise 1 binary...\n");
        execlp("./fork,PID,Wait", "./fork,PID,Wait", NULL);

        perror("exec failed");
        exit(1);
    } else {
        wait(NULL);
        printf("[Parent] Child execution through exec is complete.\n");
    }
    return 0;
}