/*
 * Orphan and Zombie process demonstration.
 *
 * Compile: gcc orphan_zombie.c -o orphan_zombie
 * Run:     ./orphan_zombie          (both demos)
 *          ./orphan_zombie zombie   (zombie only)
 *          ./orphan_zombie orphan   (orphan only)
 *
 * Zombie: a child exits but its parent does not call wait()/waitpid(), so the
 *         child's entry stays in the process table with state Z until it is
 *         reaped.
 *
 * Orphan: a parent exits while its child is still running. The child is then
 *         reparented to init (PID 1) or to a subreaper, which reaps it.
 *
 * The ps output is printed by a separate command so the states can be seen
 * while the demo is running.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void show_process(pid_t pid) {
    char cmd[160];

    printf("  ps for PID %ld:\n", (long)pid);
    fflush(stdout);
    snprintf(cmd, sizeof(cmd),
             "COLUMNS=120 LINES=24 ps -o pid=,ppid=,stat=,comm= -p %ld",
             (long)pid);
    if (system(cmd) != 0)
        printf("  (process is gone or ps is unavailable)\n");
}

static void demo_zombie(void) {
    printf("\n=== ZOMBIE PROCESS ===\n");
    fflush(stdout);         /* do not let the child inherit buffered output */

    pid_t child = fork();
    if (child < 0) {
        perror("fork zombie child");
        return;
    }

    if (child == 0) {
        printf("[zombie child] PID %d exiting immediately\n", getpid());
        fflush(stdout);
        _exit(0);
    }

    printf("[parent] child PID %d has exited, but wait() is delayed...\n",
           child);
    sleep(2);

    printf("[parent] the child should now be a zombie (STAT = Z):\n");
    show_process(child);

    int status;
    pid_t reaped = waitpid(child, &status, 0);
    if (reaped == child) {
        printf("[parent] waitpid() reaped PID %d", child);
        if (WIFEXITED(status))
            printf(" (exit status %d)", WEXITSTATUS(status));
        printf("\n");
    } else {
        perror("waitpid");
    }

    printf("[parent] after reaping, the PID no longer exists:\n");
    show_process(child);
}

static void demo_orphan(void) {
    printf("\n=== ORPHAN PROCESS ===\n");
    fflush(stdout);         /* do not let the children inherit buffered output */

    int pid_pipe[2];
    if (pipe(pid_pipe) == -1) {
        perror("pipe");
        return;
    }

    pid_t parent = fork();
    if (parent < 0) {
        perror("fork parent");
        close(pid_pipe[0]);
        close(pid_pipe[1]);
        return;
    }

    if (parent == 0) {
        /* Intermediate parent: create a child, report its PID, then exit
         * without waiting, leaving the child orphaned. */
        close(pid_pipe[0]);

        pid_t child = fork();
        if (child < 0) {
            perror("fork orphan child");
            _exit(1);
        }

        if (child == 0) {
            close(pid_pipe[1]);
            printf("[orphan child] PID %d started, PPID is %d\n",
                   getpid(), getppid());
            fflush(stdout);
            sleep(3);
            printf("[orphan child] after parent exit, PPID is now %d\n",
                   getppid());
            fflush(stdout);
            _exit(0);
        }

        ssize_t written = write(pid_pipe[1], &child, sizeof(child));
        (void)written;
        close(pid_pipe[1]);

        printf("[parent] created child PID %d and is exiting without waiting\n",
               child);
        fflush(stdout);
        _exit(0);
    }

    close(pid_pipe[1]);

    pid_t orphan = -1;
    ssize_t got;
    do {
        got = read(pid_pipe[0], &orphan, sizeof(orphan));
    } while (got < 0 && errno == EINTR);
    close(pid_pipe[0]);

    int status;
    waitpid(parent, &status, 0);

    if (got == (ssize_t)sizeof(orphan)) {
        printf("[main] intermediate parent exited; PID %d is an orphan\n",
               orphan);
        sleep(1);
        printf("[main] the orphan's parent should now be init/subreaper:\n");
        show_process(orphan);
        printf("[main] waiting for the orphan to finish...\n");
        sleep(3);
    }

    printf("[main] orphan demo complete (its new parent reaps the orphan).\n");
}

int main(int argc, char *argv[]) {
    int do_zombie = 1;
    int do_orphan = 1;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [zombie|orphan]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "zombie") == 0) {
            do_orphan = 0;
        } else if (strcmp(argv[1], "orphan") == 0) {
            do_zombie = 0;
        } else {
            fprintf(stderr, "Usage: %s [zombie|orphan]\n", argv[0]);
            return 1;
        }
    }

    printf("Orphan and Zombie Process Demonstration\n");
    printf("Main process PID: %d, PPID: %d\n", getpid(), getppid());
    fflush(stdout);

    if (do_zombie)
        demo_zombie();
    if (do_orphan)
        demo_orphan();

    return 0;
}

/*
Sample Output (PIDs and timing vary):

Orphan and Zombie Process Demonstration
Main process PID: 4201, PPID: 3100

=== ZOMBIE PROCESS ===
[zombie child] PID 4202 exiting immediately
[parent] child PID 4202 has exited, but wait() is delayed...
[parent] the child should now be a zombie (STAT = Z):
  ps for PID 4202:
   4202   4201 Z    orphan_zombie
[parent] waitpid() reaped PID 4202 (exit status 0)
[parent] after reaping, the PID no longer exists:
  ps for PID 4202:
  (process is gone or ps is unavailable)

=== ORPHAN PROCESS ===
[orphan child] PID 4204 started, PPID is 4203
[parent] created child PID 4204 and is exiting without waiting
[main] intermediate parent exited; PID 4204 is an orphan
[main] the orphan's parent should now be init/subreaper:
  ps for PID 4204:
   4204      1 S    orphan_zombie
[main] waiting for the orphan to finish...
[orphan child] after parent exit, PPID is now 1
[main] orphan demo complete (its new parent reaps the orphan).
*/
