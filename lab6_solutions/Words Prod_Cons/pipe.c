#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_WORDS 5
#define WORD_LEN  50

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);

        int n;
        printf("Producer - How many words (max %d): ", MAX_WORDS);
        scanf("%d", &n);
        if (n > MAX_WORDS) n = MAX_WORDS;

        write(fd[1], &n, sizeof(int));

        printf("Producer - Enter %d words:\n", n);
        for (int i = 0; i < n; i++) {
            char word[WORD_LEN];
            printf("  Word %d: ", i + 1);
            scanf("%s", word);
            write(fd[1], word, WORD_LEN);
            printf("Producer wrote: %s\n", word);
        }

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        wait(NULL);

        int n;
        read(fd[0], &n, sizeof(int));
        printf("\nConsumer reading %d words from pipe\n", n);
        for (int i = 0; i < n; i++) {
            char word[WORD_LEN];
            read(fd[0], word, WORD_LEN);
            printf("Consumer read: %s\n", word);
        }

        close(fd[0]);
    }

    return 0;
}
