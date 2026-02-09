#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Parent PID: %d. Child PID: %d. Parent sleeping for 20s...\n", getpid(), pid);
        printf("Run 'ps -e -o pid,ppid,state,cmd | grep %d' now to see <defunct>.\n", pid);
        sleep(20);
        printf("Parent terminating. Child will now be orphaned.\n");
        exit(0);
    } else if (pid == 0) {
        printf("Child PID: %d. Exiting to become a zombie...\n", getpid());
        exit(0); 
    }
    return 0;
}