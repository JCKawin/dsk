/*
 * Dining Philosophers using POSIX semaphores.
 * Classic solution: each philosopher picks up the lower-numbered chopstick
 * first (asymmetric / resource hierarchy). This prevents circular wait,
 * so deadlock cannot occur. A room semaphore is also available as the
 * alternative "at most N-1 seated" solution; the hierarchy version is used.
 *
 * Compile: gcc dining_philosophers.c -o dining_philosophers -pthread
 * Run:     ./dining_philosophers
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define MEALS 2

static sem_t chopstick[N];

static void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;
    /* Always acquire the lower index first -> breaks the cycle. */
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    for (int meal = 1; meal <= MEALS; meal++) {
        printf("Philosopher %d is thinking\n", id);
        usleep(100000 + (id * 20000));

        printf("Philosopher %d is hungry\n", id);
        sem_wait(&chopstick[first]);
        printf("Philosopher %d picked chopstick %d\n", id, first);
        sem_wait(&chopstick[second]);
        printf("Philosopher %d picked chopstick %d\n", id, second);

        printf("Philosopher %d is eating meal %d\n", id, meal);
        usleep(120000);

        sem_post(&chopstick[second]);
        sem_post(&chopstick[first]);
        printf("Philosopher %d put down chopsticks (finished meal %d)\n", id, meal);
    }
    printf("Philosopher %d is done\n", id);
    return NULL;
}

int main(void) {
    pthread_t tid[N];
    int id[N];

    for (int i = 0; i < N; i++)
        sem_init(&chopstick[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    for (int i = 0; i < N; i++)
        sem_destroy(&chopstick[i]);

    printf("All philosophers finished. No deadlock.\n");
    return 0;
}
