#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PRINTERS 3

sem_t printerSemaphore;
pthread_mutex_t printerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t coutMutex    = PTHREAD_MUTEX_INITIALIZER;

int printerAvailable[NUM_PRINTERS + 1];

typedef struct {
    int studentID;
    int pages;
} StudentArgs;

int allocatePrinter() {
    pthread_mutex_lock(&printerMutex);
    for (int i = 1; i <= NUM_PRINTERS; i++) {
        if (printerAvailable[i]) {
            printerAvailable[i] = 0;
            pthread_mutex_unlock(&printerMutex);
            return i;
        }
    }
    pthread_mutex_unlock(&printerMutex);
    return -1;
}

void releasePrinter(int printerNum) {
    pthread_mutex_lock(&printerMutex);
    printerAvailable[printerNum] = 1;
    pthread_mutex_unlock(&printerMutex);
}

void *studentPrint(void *arg) {
    StudentArgs *s = (StudentArgs *)arg;
    int studentID = s->studentID;
    int pages = s->pages;

    pthread_mutex_lock(&coutMutex);
    printf("Student %d requesting printer\n", studentID);
    pthread_mutex_unlock(&coutMutex);

    if (sem_trywait(&printerSemaphore) != 0) {
        pthread_mutex_lock(&coutMutex);
        printf("All printers busy. Student %d waiting...\n\n", studentID);
        pthread_mutex_unlock(&coutMutex);
        sem_wait(&printerSemaphore);
    }

    int printerNum = allocatePrinter();

    pthread_mutex_lock(&coutMutex);
    printf("Student %d allocated Printer %d\n", studentID, printerNum);
    printf("Student %d printing %d pages\n\n", studentID, pages);
    pthread_mutex_unlock(&coutMutex);

    sleep(pages / 4 + 1);

    pthread_mutex_lock(&coutMutex);
    printf("Student %d finished printing and released printer\n\n", studentID);
    pthread_mutex_unlock(&coutMutex);

    releasePrinter(printerNum);
    sem_post(&printerSemaphore);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: printerAccess <num_students> <id1> <pages1> ...\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (argc != 2 + 2 * n) {
        fprintf(stderr, "Argument count mismatch.\n");
        return 1;
    }

    StudentArgs students[n];
    for (int i = 0; i < n; i++) {
        students[i].studentID = atoi(argv[2 + 2 * i]);
        students[i].pages     = atoi(argv[3 + 2 * i]);
    }

    for (int i = 1; i <= NUM_PRINTERS; i++) printerAvailable[i] = 1;

    if (sem_init(&printerSemaphore, 0, NUM_PRINTERS) != 0) {
        perror("sem_init failed");
        return 1;
    }

    pthread_t threads[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, studentPrint, &students[i]);
        usleep(100000);
    }

    for (int i = 0; i < n; i++) pthread_join(threads[i], NULL);

    sem_destroy(&printerSemaphore);
    return 0;
}