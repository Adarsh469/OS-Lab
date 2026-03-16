#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define KEY_AB 1111
#define KEY_BA 2222

struct msg {
    long type;
    int  number;
    char result[30];
};

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
    int qid_ab = msgget((key_t)KEY_AB, 0666 | IPC_CREAT);
    int qid_ba = msgget((key_t)KEY_BA, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid == 0) {
        struct msg m;
        msgrcv(qid_ab, &m, sizeof(m) - sizeof(long), 1, 0);
        printf("Process B received: %d\n", m.number);

        struct msg reply;
        reply.type   = 1;
        reply.number = m.number;
        if (is_palindrome(m.number))
            strcpy(reply.result, "Palindrome");
        else
            strcpy(reply.result, "Not a Palindrome");

        printf("Process B sending result: %s\n", reply.result);
        msgsnd(qid_ba, &reply, sizeof(reply) - sizeof(long), 0);
        exit(0);
    } else {
        struct msg m;
        printf("Process A - Enter a number: ");
        scanf("%d", &m.number);
        m.type = 1;

        msgsnd(qid_ab, &m, sizeof(m) - sizeof(long), 0);
        printf("Process A sent: %d\n", m.number);

        struct msg reply;
        msgrcv(qid_ba, &reply, sizeof(reply) - sizeof(long), 1, 0);
        printf("Process A received result: %d is %s\n", m.number, reply.result);

        wait(NULL);
        msgctl(qid_ab, IPC_RMID, 0);
        msgctl(qid_ba, IPC_RMID, 0);
    }

    return 0;
}
