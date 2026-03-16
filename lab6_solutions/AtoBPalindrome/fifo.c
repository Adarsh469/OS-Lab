#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_AB "/tmp/fifo_ab_ex1"
#define FIFO_BA "/tmp/fifo_ba_ex1"

int is_palindrome(int n) {
    if (n < 0) return 0;
    int original = n, reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return original == reversed;
}

int main() {
    mkfifo(FIFO_AB, 0666);
    mkfifo(FIFO_BA, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int fd_r = open(FIFO_AB, O_RDONLY);
        int num;
        read(fd_r, &num, sizeof(int));
        close(fd_r);

        printf("Process B received: %d\n", num);

        char result[30];
        if (is_palindrome(num))
            strcpy(result, "Palindrome");
        else
            strcpy(result, "Not a Palindrome");

        printf("Process B sending result: %s\n", result);

        int fd_w = open(FIFO_BA, O_WRONLY);
        write(fd_w, result, strlen(result) + 1);
        close(fd_w);
        exit(0);
    } else {
        int num;
        printf("Process A - Enter a number: ");
        scanf("%d", &num);

        int fd_w = open(FIFO_AB, O_WRONLY);
        write(fd_w, &num, sizeof(int));
        close(fd_w);
        printf("Process A sent: %d\n", num);

        int fd_r = open(FIFO_BA, O_RDONLY);
        char result[30];
        read(fd_r, result, sizeof(result));
        close(fd_r);
        printf("Process A received result: %d is %s\n", num, result);

        wait(NULL);
        unlink(FIFO_AB);
        unlink(FIFO_BA);
    }

    return 0;
}
