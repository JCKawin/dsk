/*
 * Producer-Consumer using POSIX semaphores + a shared circular buffer.
 * One producer process, one consumer process (fork).
 *
 * Compile: gcc producer_consumer.c -o producer_consumer -pthread
 * Run:     ./producer_consumer
 *
 * Semaphores:
 *   empty  - number of free slots (init = BUFFER_SIZE)
 *   full   - number of filled slots (init = 0)
 *   mutex  - mutual exclusion on the buffer (init = 1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define ITEMS       10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t empty;
    sem_t full;
    sem_t mutex;
} Shared;

static Shared *create_shared(void) {
    Shared *s = mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (s == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    s->in = s->out = 0;
    /* pshared = 1 so the semaphore is visible across fork */
    sem_init(&s->empty, 1, BUFFER_SIZE);
    sem_init(&s->full,  1, 0);
    sem_init(&s->mutex, 1, 1);
    return s;
}

int main(void) {
    Shared *s = create_shared();
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Consumer */
        for (int i = 0; i < ITEMS; i++) {
            sem_wait(&s->full);
            sem_wait(&s->mutex);

            int item = s->buffer[s->out];
            printf("[consumer] consumed %d from slot %d\n", item, s->out);
            s->out = (s->out + 1) % BUFFER_SIZE;

            sem_post(&s->mutex);
            sem_post(&s->empty);
            usleep(150000);
        }
        exit(0);
    }

    /* Producer */
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&s->empty);
        sem_wait(&s->mutex);

        s->buffer[s->in] = i * 10;
        printf("[producer] produced %d into slot %d\n", s->buffer[s->in], s->in);
        s->in = (s->in + 1) % BUFFER_SIZE;

        sem_post(&s->mutex);
        sem_post(&s->full);
        usleep(80000);
    }

    waitpid(pid, NULL, 0);
    sem_destroy(&s->empty);
    sem_destroy(&s->full);
    sem_destroy(&s->mutex);
    munmap(s, sizeof(Shared));
    printf("Producer-consumer finished (%d items).\n", ITEMS);
    return 0;
}
