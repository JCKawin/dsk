/*
 * Producer-Consumer using an anonymous pipe + POSIX semaphores.
 * One producer process, one consumer process (fork).
 *
 * Compile: gcc producer_consumer_pipe.c -o producer_consumer_pipe -pthread
 * Run:     ./producer_consumer_pipe
 *
 * The pipe carries the items; the semaphores make the pipe behave like a
 * bounded buffer of BUFFER_SIZE slots:
 *   empty  - free slots             (init = BUFFER_SIZE)
 *   full   - filled slots           (init = 0)
 *   mutex  - serialises access to the logical buffer (init = 1)
 *
 * The pipe's kernel buffer is larger than BUFFER_SIZE, so "empty" is what
 * actually limits how many items can be produced ahead of the consumer.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define ITEMS       10

typedef struct {
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
    /* pshared = 1 so the semaphores are visible across fork */
    sem_init(&s->empty, 1, BUFFER_SIZE);
    sem_init(&s->full,  1, 0);
    sem_init(&s->mutex, 1, 1);
    return s;
}

static int write_item(int fd, int item) {
    const char *p = (const char *)&item;
    size_t left = sizeof(item);
    while (left > 0) {
        ssize_t n = write(fd, p, left);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += n;
        left -= (size_t)n;
    }
    return 0;
}

static int read_item(int fd, int *item) {
    char *p = (char *)item;
    size_t left = sizeof(*item);
    while (left > 0) {
        ssize_t n = read(fd, p, left);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0)
            return -1;          /* writer closed the pipe */
        p += n;
        left -= (size_t)n;
    }
    return 0;
}

int main(void) {
    Shared *s = create_shared();
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        sem_destroy(&s->empty);
        sem_destroy(&s->full);
        sem_destroy(&s->mutex);
        munmap(s, sizeof(Shared));
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        sem_destroy(&s->empty);
        sem_destroy(&s->full);
        sem_destroy(&s->mutex);
        munmap(s, sizeof(Shared));
        return 1;
    }

    if (pid == 0) {
        /* Consumer: read ITEMS values from the pipe. */
        int item;
        close(fd[1]);
        for (int i = 0; i < ITEMS; i++) {
            sem_wait(&s->full);
            sem_wait(&s->mutex);

            if (read_item(fd[0], &item) == -1) {
                perror("read");
                sem_post(&s->mutex);
                sem_post(&s->empty);
                close(fd[0]);
                exit(1);
            }
            printf("[consumer] consumed %d (%d/%d)\n", item, i + 1, ITEMS);

            sem_post(&s->mutex);
            sem_post(&s->empty);
            usleep(150000);
        }
        close(fd[0]);
        exit(0);
    }

    /* Producer: write ITEMS values into the pipe. */
    close(fd[0]);
    for (int i = 1; i <= ITEMS; i++) {
        int item = i * 10;

        sem_wait(&s->empty);
        sem_wait(&s->mutex);

        if (write_item(fd[1], item) == -1) {
            perror("write");
            sem_post(&s->mutex);
            sem_post(&s->full);
            close(fd[1]);
            waitpid(pid, NULL, 0);
            sem_destroy(&s->empty);
            sem_destroy(&s->full);
            sem_destroy(&s->mutex);
            munmap(s, sizeof(Shared));
            return 1;
        }
        printf("[producer] produced %d (%d/%d)\n", item, i, ITEMS);

        sem_post(&s->mutex);
        sem_post(&s->full);
        usleep(80000);
    }
    close(fd[1]);

    waitpid(pid, NULL, 0);

    sem_destroy(&s->empty);
    sem_destroy(&s->full);
    sem_destroy(&s->mutex);
    munmap(s, sizeof(Shared));

    printf("Producer-consumer finished (%d items through a pipe).\n", ITEMS);
    return 0;
}

/*
Sample Output (ordering may vary because the two processes run concurrently):

[producer] produced 10 (1/10)
[producer] produced 20 (2/10)
[producer] produced 30 (3/10)
[producer] produced 40 (4/10)
[producer] produced 50 (5/10)
[consumer] consumed 10 (1/10)
[producer] produced 60 (6/10)
[consumer] consumed 20 (2/10)
...
[consumer] consumed 100 (10/10)
Producer-consumer finished (10 items through a pipe).

Note: the producer never gets more than BUFFER_SIZE = 5 items ahead of the
consumer, which is enforced by the "empty" semaphore.
*/
