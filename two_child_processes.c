/*
 * Process Creation: Creating two child processes
 * Displays the PID and PPID of both child processes and the parent process.
 *
 * Compile: gcc two_child_processes.c -o two_child_processes
 * Run:     ./two_child_processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid1, pid2;

    printf("=========================================\n");
    printf("  Process Creation: Two Child Processes  \n");
    printf("=========================================\n");
    printf("Parent process started. PID: %d, PPID: %d\n\n", getpid(), getppid());

    /* Create first child process */
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed for Child 1");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        /* First child process */
        printf("[Child 1] Created successfully!\n");
        printf("[Child 1] Process ID (PID)        : %d\n", getpid());
        printf("[Child 1] Parent Process ID (PPID): %d\n\n", getppid());
        exit(EXIT_SUCCESS);
    }

    /* Create second child process from the parent */
    pid2 = fork();

    if (pid2 < 0) {
        perror("Fork failed for Child 2");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        /* Second child process */
        printf("[Child 2] Created successfully!\n");
        printf("[Child 2] Process ID (PID)        : %d\n", getpid());
        printf("[Child 2] Parent Process ID (PPID): %d\n\n", getppid());
        exit(EXIT_SUCCESS);
    }

    /* Parent process waits for both child processes to complete */
    wait(NULL);
    wait(NULL);

    printf("[Parent ] Both children have finished execution.\n");
    printf("[Parent ] Process ID (PID)        : %d\n", getpid());
    printf("[Parent ] Parent Process ID (PPID): %d\n", getppid());
    printf("=========================================\n");

    return 0;
}
