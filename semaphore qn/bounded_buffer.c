#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 4
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_EACH 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 1; i <= ITEMS_EACH; i++) {
        int item = id * 100 + i;

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer %d inserted item %d into slot %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    for (int i = 1; i <= ITEMS_EACH; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        printf("Consumer %d removed  item %d from slot %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        sleep(2);
    }
    return NULL;
}

int main() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full,  0, 0);
    sem_init(&mutex, 0, 1);

    int pids[NUM_PRODUCERS], cids[NUM_CONSUMERS];
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &pids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &cids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(producers[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(consumers[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
