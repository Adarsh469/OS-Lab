#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define KEY_PC 6666
#define KEY_CP 7777

struct msg {
    long type;
    char letter;
};

int main() {
    int qid_pc = msgget((key_t)KEY_PC, 0666 | IPC_CREAT);
    int qid_cp = msgget((key_t)KEY_CP, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        struct msg m, reply;
        msgrcv(qid_pc, &m, sizeof(m) - sizeof(long), 1, 0);
        printf("Child received: %c\n", m.letter);

        reply.type   = 1;
        reply.letter = m.letter + 1;
        printf("Child sending: %c\n", reply.letter);
        msgsnd(qid_cp, &reply, sizeof(reply) - sizeof(long), 0);
        exit(0);
    } else {
        struct msg m, reply;
        printf("Parent - Enter an alphabet: ");
        scanf(" %c", &m.letter);
        m.type = 1;

        msgsnd(qid_pc, &m, sizeof(m) - sizeof(long), 0);
        printf("Parent sent: %c\n", m.letter);

        msgrcv(qid_cp, &reply, sizeof(reply) - sizeof(long), 1, 0);
        printf("Parent received reply from child: %c\n", reply.letter);

        wait(NULL);
        msgctl(qid_pc, IPC_RMID, 0);
        msgctl(qid_cp, IPC_RMID, 0);
    }

    return 0;
}
