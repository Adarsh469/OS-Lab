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

    msgrcv(msgid, &message, sizeof(message.number), 1, 0);

    int num = message.number;
    int reversed = 0, temp = num;
    while (temp != 0) {
        reversed = reversed * 10 + temp % 10;
        temp /= 10;
    }

    printf("Process B received: %d\n", num);
    if (num == reversed) printf("Result: It is a Palindrome.\n");
    else printf("Result: Not a Palindrome.\n");

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}