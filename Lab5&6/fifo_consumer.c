#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char *myfifo = "/tmp/int_fifo";
    int received_nums[4];

    fd = open(myfifo, O_RDONLY);
    read(fd, received_nums, sizeof(received_nums));
    
    printf("Consumer received: ");
    for(int i=0; i<4; i++) printf("%d ", received_nums[i]);
    printf("\n");

    close(fd);
    unlink(myfifo);
    return 0;
}