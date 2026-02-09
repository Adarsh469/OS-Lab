#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fib_arr;
int count;

void* generate_fib(void* param) {
    if (count > 0) fib_arr[0] = 0;
    if (count > 1) fib_arr[1] = 1;
    for (int i = 2; i < count; i++) {
        fib_arr[i] = fib_arr[i-1] + fib_arr[i-2];
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./fib <number>\n");
        return 1;
    }

    count = atoi(argv[1]);
    fib_arr = malloc(count * sizeof(int));

    pthread_t tid;
    pthread_create(&tid, NULL, generate_fib, NULL);
    pthread_join(tid, NULL);

    printf("Fibonacci Sequence: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", fib_arr[i]);
    }
    printf("\n");

    free(fib_arr);
    return 0;
}