#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_data = 0;
int reader_count = 0;

sem_t mutex;
sem_t write_lock;

void *reader(void *arg) {
    int id = *(int *)arg;

    sem_wait(&mutex);
    reader_count++;
    if (reader_count == 1)
        sem_wait(&write_lock);
    sem_post(&mutex);

    printf("Reader %d is reading  -> shared data = %d\n", id, shared_data);
    sleep(1);

    sem_wait(&mutex);
    reader_count--;
    if (reader_count == 0)
        sem_post(&write_lock);
    sem_post(&mutex);

    printf("Reader %d finished reading\n", id);
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    sem_wait(&write_lock);

    shared_data += 10;
    printf("Writer %d is writing  -> shared data = %d\n", id, shared_data);
    sleep(1);

    sem_post(&write_lock);

    printf("Writer %d finished writing\n", id);
    return NULL;
}

int main() {
    sem_init(&mutex,      0, 1);
    sem_init(&write_lock, 0, 1);

    int rids[5] = {1, 2, 3, 4, 5};
    int wids[3] = {1, 2, 3};

    pthread_t readers[5], writers[3];

    for (int i = 0; i < 5; i++) pthread_create(&readers[i], NULL, reader, &rids[i]);
    for (int i = 0; i < 3; i++) pthread_create(&writers[i], NULL, writer, &wids[i]);

    for (int i = 0; i < 5; i++) pthread_join(readers[i], NULL);
    for (int i = 0; i < 3; i++) pthread_join(writers[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&write_lock);

    return 0;
}
