#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PC "/tmp/fifo_pc_ex3"
#define FIFO_CP "/tmp/fifo_cp_ex3"

int main() {
    mkfifo(FIFO_PC, 0666);
    mkfifo(FIFO_CP, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int fd_r = open(FIFO_PC, O_RDONLY);
        char letter;
        read(fd_r, &letter, sizeof(char));
        close(fd_r);
        printf("Child received: %c\n", letter);

        char reply = letter + 1;
        printf("Child sending: %c\n", reply);

        int fd_w = open(FIFO_CP, O_WRONLY);
        write(fd_w, &reply, sizeof(char));
        close(fd_w);
        exit(0);
    } else {
        char letter;
        printf("Parent - Enter an alphabet: ");
        scanf(" %c", &letter);

        int fd_w = open(FIFO_PC, O_WRONLY);
        write(fd_w, &letter, sizeof(char));
        close(fd_w);
        printf("Parent sent: %c\n", letter);

        int fd_r = open(FIFO_CP, O_RDONLY);
        char reply;
        read(fd_r, &reply, sizeof(char));
        close(fd_r);
        printf("Parent received reply from child: %c\n", reply);

        wait(NULL);
        unlink(FIFO_PC);
        unlink(FIFO_CP);
    }

    return 0;
}
