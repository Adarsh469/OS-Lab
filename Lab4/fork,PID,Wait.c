#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        printf("[Child]  Hello! I am the child process.\n");
        printf("[Child]  My PID: %d, My Parent's PID: %d\n", getpid(), getppid());
        exit(0);
    } 
    else {
        printf("[Parent] Hello! I am the parent process.\n");
        printf("[Parent] My PID: %d, My Child's PID: %d\n", getpid(), pid);
        
        printf("[Parent] Waiting for child to complete...\n");
        wait(NULL); 
        printf("[Parent] Child finished. Parent exiting.\n");
    }

    return 0;
}