#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/fifo_ex4"
#define MAX_WORDS 5
#define WORD_LEN  50

int main() {
    mkfifo(FIFO_PATH, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        int fd = open(FIFO_PATH, O_WRONLY);

        int n;
        printf("Producer - How many words (max %d): ", MAX_WORDS);
        scanf("%d", &n);
        if (n > MAX_WORDS) n = MAX_WORDS;

        write(fd, &n, sizeof(int));

        printf("Producer - Enter %d words:\n", n);
        for (int i = 0; i < n; i++) {
            char word[WORD_LEN];
            printf("  Word %d: ", i + 1);
            scanf("%s", word);
            write(fd, word, WORD_LEN);
            printf("Producer wrote: %s\n", word);
        }

        close(fd);
        exit(0);
    } else {
        int fd = open(FIFO_PATH, O_RDONLY);

        int n;
        read(fd, &n, sizeof(int));
        printf("\nConsumer reading %d words from FIFO\n", n);
        for (int i = 0; i < n; i++) {
            char word[WORD_LEN];
            read(fd, word, WORD_LEN);
            printf("Consumer read: %s\n", word);
        }

        close(fd);
        wait(NULL);
        unlink(FIFO_PATH);
    }

    return 0;
}
