#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char *myfifo = "/tmp/int_fifo";
    mkfifo(myfifo, 0666);
    int nums[4] = {10, 20, 30, 40};

    fd = open(myfifo, O_WRONLY);
    write(fd, nums, sizeof(nums));
    close(fd);

    printf("Producer: Sent 4 integers into FIFO.\n");
    return 0;
}