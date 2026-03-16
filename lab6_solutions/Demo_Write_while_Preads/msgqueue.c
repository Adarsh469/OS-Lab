#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY 4040

struct msg {
    long type;
    char text[256];
};

int main() {
    int qid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        struct msg m;
        m.type = 1;
        strcpy(m.text, "Message from child process through message queue!");
        printf("Child (PID %d): writing -> \"%s\"\n", getpid(), m.text);
        msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);
        exit(0);
    } else {
        wait(NULL);

        struct msg m;
        msgrcv(qid, &m, sizeof(m) - sizeof(long), 1, 0);
        printf("Parent (PID %d): read   -> \"%s\"\n", getpid(), m.text);

        msgctl(qid, IPC_RMID, 0);
    }

    return 0;
}
