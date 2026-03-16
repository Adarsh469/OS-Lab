#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[2];
    char msg[] = "Hello through a pipe!";
    char buf[100];
    int n;

    pipe(fd);
    printf("Pipe created: fd[0]=%d (read), fd[1]=%d (write)\n\n", fd[0], fd[1]);

    printf("Writing: \"%s\"\n", msg);
    write(fd[1], msg, strlen(msg));

    n = read(fd[0], buf, sizeof(buf) - 1);
    buf[n] = '\0';
    printf("Reading: \"%s\"\n", buf);
    printf("Bytes read: %d\n", n);

    close(fd[0]);
    close(fd[1]);
    return 0;
}
