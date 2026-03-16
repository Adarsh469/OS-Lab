#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY 4444
#define COUNT   4

struct msg {
    long type;
    int  number;
};

int main() {
    int qid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        int numbers[COUNT] = {10, 20, 30, 40};
        printf("Producer writing %d integers into message queue\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            struct msg m;
            m.type   = 1;
            m.number = numbers[i];
            msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);
            printf("Producer wrote: %d\n", numbers[i]);
        }
        exit(0);
    } else {
        wait(NULL);

        printf("\nConsumer reading %d integers from message queue\n", COUNT);
        for (int i = 0; i < COUNT; i++) {
            struct msg m;
            msgrcv(qid, &m, sizeof(m) - sizeof(long), 1, 0);
            printf("Consumer read: %d\n", m.number);
        }

        msgctl(qid, IPC_RMID, 0);
    }

    return 0;
}
