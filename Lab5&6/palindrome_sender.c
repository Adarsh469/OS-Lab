#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buf {
    long msg_type;
    int number;
};

int main() {
    key_t key = ftok("palin_msg", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct msg_buf message;

    message.msg_type = 1;
    printf("Process A: Enter a number to send: ");
    scanf("%d", &message.number);

    msgsnd(msgid, &message, sizeof(message.number), 0);
    printf("Process A: Number sent.\n");

    return 0;
}