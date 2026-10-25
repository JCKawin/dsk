/*
 * Readers-Writers (first readers-writers problem: readers preference)
 * using POSIX semaphores. Threads share the data structure.
 *
 * Compile: gcc readers_writers.c -o readers_writers -pthread
 * Run:     ./readers_writers
 *
 * mutex     protects read_count
 * write_lock gives a writer exclusive access; the first reader acquires it
 *            and the last reader releases it, so writers wait while any
 *            reader is active.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 4
#define WRITERS 2
#define READS_EACH  3
#define WRITES_EACH 2

static int shared_data = 0;
static int read_count = 0;
static sem_t mutex;
static sem_t write_lock;

static void *reader(void *arg) {
    int id = *(int *)arg;
    for (int k = 0; k < READS_EACH; k++) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&write_lock);   /* first reader locks out writers */
        sem_post(&mutex);

        printf("Reader %d reads value %d (active readers=%d)\n",
               id, shared_data, read_count);
        usleep(100000);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&write_lock);   /* last reader lets writers in */
        sem_post(&mutex);
        usleep(50000);
    }
    return NULL;
}

static void *writer(void *arg) {
    int id = *(int *)arg;
    for (int k = 0; k < WRITES_EACH; k++) {
        sem_wait(&write_lock);
        shared_data += 1;
        printf("Writer %d writes value %d\n", id, shared_data);
        usleep(150000);
        sem_post(&write_lock);
        usleep(80000);
    }
    return NULL;
}

int main(void) {
    pthread_t rt[READERS], wt[WRITERS];
    int rid[READERS], wid[WRITERS];

    sem_init(&mutex, 0, 1);
    sem_init(&write_lock, 0, 1);

    for (int i = 0; i < READERS; i++) {
        rid[i] = i + 1;
        pthread_create(&rt[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < WRITERS; i++) {
        wid[i] = i + 1;
        pthread_create(&wt[i], NULL, writer, &wid[i]);
    }
    for (int i = 0; i < READERS; i++) pthread_join(rt[i], NULL);
    for (int i = 0; i < WRITERS; i++) pthread_join(wt[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&write_lock);
    printf("Final shared value = %d (expected %d)\n", shared_data, WRITERS * WRITES_EACH);
    return 0;
}
