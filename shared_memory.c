/*
 * Shared memory: parent creates a POSIX shared-memory segment,
 * writes a message, child attaches and reads it, then parent unlinks.
 *
 * Compile: gcc shared_memory.c -o shared_memory -lrt
 * Run:     ./shared_memory
 *
 * POSIX shm is used (shm_open / mmap). Works on Linux.
 * The segment name is /oslab_shm_demo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define SHM_NAME "/oslab_shm_demo"
#define SHM_SIZE 256

int main(void) {
    /* Clean leftover name from a previous crash, then create fresh. */
    shm_unlink(SHM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    char *shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap parent");
        return 1;
    }
    close(fd); /* mapping remains valid */

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Child: attach to the same segment and read. */
        int cfd = shm_open(SHM_NAME, O_RDONLY, 0666);
        if (cfd == -1) {
            perror("child shm_open");
            exit(1);
        }
        char *cshm = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, cfd, 0);
        if (cshm == MAP_FAILED) {
            perror("child mmap");
            exit(1);
        }
        close(cfd);

        /* Wait until parent publishes a non-empty string. */
        while (cshm[0] == '\0')
            usleep(10000);

        printf("[child  pid=%d] read from shared memory: \"%s\"\n", getpid(), cshm);
        munmap(cshm, SHM_SIZE);
        exit(0);
    }

    /* Parent writes after a short pause so the child is waiting. */
    const char *msg = "Hello from parent via POSIX shared memory";
    printf("[parent pid=%d] writing to shared memory...\n", getpid());
    usleep(50000);
    snprintf(shm, SHM_SIZE, "%s", msg);
    printf("[parent pid=%d] wrote: \"%s\"\n", getpid(), shm);

    int status = 0;
    waitpid(pid, &status, 0);

    munmap(shm, SHM_SIZE);
    if (shm_unlink(SHM_NAME) == -1)
        perror("shm_unlink");
    else
        printf("[parent] shared memory unlinked\n");

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
