#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t fork_sem[NUM_PHILOSOPHERS];
sem_t mutex;

void think(int id) {
    printf("Philosopher %d is thinking\n", id);
    sleep(1);
}

void eat(int id) {
    printf("Philosopher %d is eating\n", id);
    sleep(2);
}

void pick_up_forks(int id) {
    int left  = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    if (id == NUM_PHILOSOPHERS - 1) {
        sem_wait(&mutex);
        sem_wait(&fork_sem[right]);
        sem_wait(&fork_sem[left]);
        sem_post(&mutex);
    } else {
        sem_wait(&mutex);
        sem_wait(&fork_sem[left]);
        sem_wait(&fork_sem[right]);
        sem_post(&mutex);
    }

    printf("Philosopher %d picked up forks %d and %d\n", id, left, right);
}

void put_down_forks(int id) {
    int left  = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    sem_post(&fork_sem[left]);
    sem_post(&fork_sem[right]);

    printf("Philosopher %d put down forks %d and %d\n", id, left, right);
}

void *philosopher(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 3; i++) {
        think(id);
        pick_up_forks(id);
        eat(id);
        put_down_forks(id);
    }

    printf("Philosopher %d is done\n", id);
    return NULL;
}

int main() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        sem_init(&fork_sem[i], 0, 1);

    sem_init(&mutex, 0, 1);

    int ids[NUM_PHILOSOPHERS];
    pthread_t threads[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(threads[i], NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        sem_destroy(&fork_sem[i]);

    sem_destroy(&mutex);

    return 0;
}
