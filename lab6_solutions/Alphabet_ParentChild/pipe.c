#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe_pc[2], pipe_cp[2];
    pipe(pipe_pc);
    pipe(pipe_cp);

    pid_t pid = fork();

    if (pid == 0) {
        close(pipe_pc[1]);
        close(pipe_cp[0]);

        char letter;
        read(pipe_pc[0], &letter, sizeof(char));
        printf("Child received: %c\n", letter);

        char reply = letter + 1;
        printf("Child sending: %c\n", reply);
        write(pipe_cp[1], &reply, sizeof(char));

        close(pipe_pc[0]);
        close(pipe_cp[1]);
        exit(0);
    } else {
        close(pipe_pc[0]);
        close(pipe_cp[1]);

        char letter;
        printf("Parent - Enter an alphabet: ");
        scanf(" %c", &letter);

        write(pipe_pc[1], &letter, sizeof(char));
        printf("Parent sent: %c\n", letter);

        char reply;
        read(pipe_cp[0], &reply, sizeof(char));
        printf("Parent received reply from child: %c\n", reply);

        close(pipe_pc[1]);
        close(pipe_cp[0]);
        wait(NULL);
    }

    return 0;
}
