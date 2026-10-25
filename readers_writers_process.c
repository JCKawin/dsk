/*
 * Readers-Writers (first readers-writers problem: readers preference)
 * using forked processes, process-shared POSIX semaphores and a pipe.
 *
 * Compile: gcc readers_writers_process.c -o readers_writers_process -pthread
 * Run:     ./readers_writers_process
 *
 * Shared state lives in an anonymous MAP_SHARED mapping, so the parent and
 * all children see the same data and counters. The semaphores are placed in
 * that mapping with pshared = 1:
 *
 *   mutex     protects read_count
 *   rw_mutex  exclusive access for writers. The first reader acquires it and
 *             the last reader releases it, so any number of readers can read
 *             together but a writer needs exclusive access.
 *
 * An anonymous pipe is used as an event log: every reader and writer sends a
 * short record to the parent, which prints the records as they arrive. This
 * shows that reads overlap while writes stay serialised.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define READERS     4
#define WRITERS     2
#define READS_EACH  3
#define WRITES_EACH 2

typedef struct {
    int data;
    int read_count;
    sem_t mutex;
    sem_t rw_mutex;
} Shared;

static void log_msg(int fd, const char *msg) {
    size_t left = strlen(msg);
    const char *p = msg;

    while (left > 0) {
        ssize_t n = write(fd, p, left);
        if (n < 0) {
            if (errno == EINTR) continue;
            return;
        }
        p += n;
        left -= (size_t)n;
    }
}

static void reader(Shared *s, int id, int log_fd) {
    char msg[160];

    for (int k = 0; k < READS_EACH; k++) {
        sem_wait(&s->mutex);
        s->read_count++;
        if (s->read_count == 1)
            sem_wait(&s->rw_mutex);   /* first reader locks out writers */
        int active = s->read_count;
        sem_post(&s->mutex);

        int value = s->data;
        snprintf(msg, sizeof(msg),
                 "[reader %d] reads value %d (active readers=%d)\n",
                 id, value, active);
        log_msg(log_fd, msg);
        usleep(120000);               /* keep the read section open */

        sem_wait(&s->mutex);
        s->read_count--;
        if (s->read_count == 0)
            sem_post(&s->rw_mutex);   /* last reader lets writers in */
        sem_post(&s->mutex);

        usleep(60000);
    }
}

static void writer(Shared *s, int id, int log_fd) {
    char msg[160];

    for (int k = 0; k < WRITES_EACH; k++) {
        sem_wait(&s->rw_mutex);

        s->data += 1;
        snprintf(msg, sizeof(msg), "[writer %d] writes value %d\n",
                 id, s->data);
        log_msg(log_fd, msg);
        usleep(150000);               /* keep the write section open */

        sem_post(&s->rw_mutex);
        usleep(80000);
    }
}

static Shared *create_shared(void) {
    Shared *s = mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (s == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    s->data = 0;
    s->read_count = 0;
    sem_init(&s->mutex, 1, 1);
    sem_init(&s->rw_mutex, 1, 1);
    return s;
}

int main(void) {
    Shared *s = create_shared();
    int log_pipe[2];

    if (pipe(log_pipe) == -1) {
        perror("pipe");
        sem_destroy(&s->mutex);
        sem_destroy(&s->rw_mutex);
        munmap(s, sizeof(Shared));
        return 1;
    }

    printf("Readers-Writers: %d readers x %d reads, %d writers x %d writes\n",
           READERS, READS_EACH, WRITERS, WRITES_EACH);
    printf("Reader/writer events are forwarded to the parent through a pipe:\n\n");
    fflush(stdout);

    /* Create the reader processes. */
    for (int i = 0; i < READERS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork reader");
            return 1;
        }
        if (pid == 0) {
            close(log_pipe[0]);
            reader(s, i + 1, log_pipe[1]);
            close(log_pipe[1]);
            _exit(0);
        }
    }

    /* Create the writer processes. */
    for (int i = 0; i < WRITERS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork writer");
            return 1;
        }
        if (pid == 0) {
            close(log_pipe[0]);
            writer(s, i + 1, log_pipe[1]);
            close(log_pipe[1]);
            _exit(0);
        }
    }

    /* Parent becomes the log collector: drain the pipe until every child has
     * closed its copy of the write end. */
    close(log_pipe[1]);
    char buf[256];
    ssize_t n;
    while ((n = read(log_pipe[0], buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        fputs(buf, stdout);
    }
    close(log_pipe[0]);

    /* Reap all children. */
    for (int i = 0; i < READERS + WRITERS; i++)
        wait(NULL);

    printf("\nFinal shared value = %d (expected %d)\n",
           s->data, WRITERS * WRITES_EACH);

    sem_destroy(&s->mutex);
    sem_destroy(&s->rw_mutex);
    munmap(s, sizeof(Shared));
    return 0;
}

/*
Sample Output (interleaving may vary):

Readers-Writers: 4 readers x 3 reads, 2 writers x 2 writes
Reader/writer events are forwarded to the parent through a pipe:

[reader 1] reads value 0 (active readers=1)
[reader 2] reads value 0 (active readers=2)
[reader 3] reads value 0 (active readers=3)
[reader 4] reads value 0 (active readers=4)
[writer 1] writes value 1
[reader 1] reads value 1 (active readers=1)
[reader 2] reads value 1 (active readers=2)
...
[writer 2] writes value 4
[reader 3] reads value 4 (active readers=1)
[reader 4] reads value 4 (active readers=2)
...
Final shared value = 4 (expected 4)

Notice that writers never overlap with readers or with each other, while
reader records can show more than one active reader at the same time.
*/
