/*
 * Process Creation using the fork() system call.
 * Demonstrates:
 *   - Creating a child process with fork()
 *   - Distinguishing parent and child execution flows
 *   - Fetching PID and PPID using getpid() and getppid()
 *   - Separate address space demonstration (local/global variables)
 *   - Synchronizing parent with child using wait() and checking exit status
 *
 * Compile: gcc process_creation.c -o process_creation
 * Run:     ./process_creation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_var = 100;

int main(void) {
    pid_t pid;
    int local_var = 20;

    printf("=========================================\n");
    printf("        Process Creation Demo            \n");
    printf("=========================================\n");
    printf("[Main] Initial Process PID: %d, PPID: %d\n", getpid(), getppid());
    printf("[Main] Initial values -> global_var: %d, local_var: %d\n\n", global_var, local_var);

    /* Fork a child process */
    pid = fork();

    if (pid < 0) {
        /* Fork failed */
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process execution */
        printf("[Child ] Created successfully!\n");
        printf("[Child ] Current PID  : %d\n", getpid());
        printf("[Child ] Parent PID   : %d\n", getppid());

        /* Modifying variables in child to demonstrate separate address space */
        global_var += 50;
        local_var += 50;
        printf("[Child ] Modified values -> global_var: %d, local_var: %d\n", global_var, local_var);
        printf("[Child ] Working on child task...\n");
        sleep(1);
        printf("[Child ] Task completed. Exiting with status 42.\n\n");
        exit(42);
    } else {
        /* Parent process execution */
        int status;
        printf("[Parent] Fork returned child PID: %d\n", pid);
        printf("[Parent] Waiting for child process to complete...\n\n");

        /* Wait for child to exit */
        pid_t terminated_pid = wait(&status);

        if (terminated_pid == -1) {
            perror("Wait failed");
            exit(EXIT_FAILURE);
        }

        printf("[Parent] Child process (PID: %d) has finished.\n", terminated_pid);
        if (WIFEXITED(status)) {
            printf("[Parent] Child exited normally with exit code: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[Parent] Child was terminated by signal: %d\n", WTERMSIG(status));
        }

        /* Show that parent's memory was unaffected by child's modifications */
        printf("[Parent] Values in parent -> global_var: %d, local_var: %d (unchanged!)\n", global_var, local_var);
        printf("=========================================\n");
    }

    return EXIT_SUCCESS;
}
