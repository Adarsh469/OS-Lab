#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
    int pipe_ab[2], pipe_ba[2];
    pipe(pipe_ab);
    pipe(pipe_ba);

    pid_t pid = fork();

    if (pid == 0) {
        close(pipe_ab[1]);
        close(pipe_ba[0]);

        int num;
        read(pipe_ab[0], &num, sizeof(int));
        printf("Process B received: %d\n", num);

        char result[30];
        if (is_palindrome(num))
            strcpy(result, "Palindrome");
        else
            strcpy(result, "Not a Palindrome");

        printf("Process B sending result: %s\n", result);
        write(pipe_ba[1], result, strlen(result) + 1);

        close(pipe_ab[0]);
        close(pipe_ba[1]);
        exit(0);
    } else {
        close(pipe_ab[0]);
        close(pipe_ba[1]);

        int num;
        printf("Process A - Enter a number: ");
        scanf("%d", &num);

        write(pipe_ab[1], &num, sizeof(int));
        printf("Process A sent: %d\n", num);

        char result[30];
        read(pipe_ba[0], result, sizeof(result));
        printf("Process A received result: %d is %s\n", num, result);

        close(pipe_ab[1]);
        close(pipe_ba[0]);
        wait(NULL);
    }

    return 0;
}
