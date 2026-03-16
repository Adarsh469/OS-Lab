#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY  9999
#define MAX_WORDS 5
#define WORD_LEN  50

struct msg {
    long type;
    char word[WORD_LEN];
};

int main() {
    int qid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        int n;
        printf("Producer - How many words (max %d): ", MAX_WORDS);
        scanf("%d", &n);
        if (n > MAX_WORDS) n = MAX_WORDS;

        struct msg count_msg;
        count_msg.type = 1;
        sprintf(count_msg.word, "%d", n);
        msgsnd(qid, &count_msg, sizeof(count_msg) - sizeof(long), 0);

        printf("Producer - Enter %d words:\n", n);
        for (int i = 0; i < n; i++) {
            struct msg m;
            m.type = 2;
            printf("  Word %d: ", i + 1);
            scanf("%s", m.word);
            msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);
            printf("Producer wrote: %s\n", m.word);
        }
        exit(0);
    } else {
        wait(NULL);

        struct msg count_msg;
        msgrcv(qid, &count_msg, sizeof(count_msg) - sizeof(long), 1, 0);
        int n = atoi(count_msg.word);

        printf("\nConsumer reading %d words from message queue\n", n);
        for (int i = 0; i < n; i++) {
            struct msg m;
            msgrcv(qid, &m, sizeof(m) - sizeof(long), 2, 0);
            printf("Consumer read: %s\n", m.word);
        }

        msgctl(qid, IPC_RMID, 0);
    }

    return 0;
}
