#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY 2020

struct msg {
    long type;
    char text[100];
};

int main() {
    int qid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
    printf("Message queue created with id: %d\n\n", qid);

    struct msg m;
    m.type = 1;
    strcpy(m.text, "Hello through a message queue!");

    printf("Writing: \"%s\"\n", m.text);
    msgsnd(qid, &m, sizeof(m) - sizeof(long), 0);

    struct msg recv;
    msgrcv(qid, &recv, sizeof(recv) - sizeof(long), 1, 0);
    printf("Reading: \"%s\"\n", recv.text);

    msgctl(qid, IPC_RMID, 0);
    printf("Message queue deleted.\n");
    return 0;
}
