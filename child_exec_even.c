/*
 * Child process executes even numbers generation between 1 to 50 using exec family function.
 * This program demonstrates using execlp (or execv/execl) to run a helper program or a command.
 *
 * It uses execlp to run either a custom binary if compiled or a standard shell/python command,
 * or it can execute a dedicated child binary ./print_evens.
 * Here we provide the complete self-contained demo using execlp with a bash/sh command,
 * while also demonstrating execl.
 *
 * Compile: gcc child_exec_even.c -o child_exec_even
 * Run:     ./child_exec_even
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;

    printf("=====================================================\n");
    printf("  Child Process Executing Even Numbers (1 to 50)     \n");
    printf("=====================================================\n");
    printf("Parent Process (PID: %d) creating child...\n\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child Process */
        printf("[Child PID: %d] Executing program using execlp()...\n", getpid());
        printf("[Child PID: %d] Even numbers between 1 and 50:\n", getpid());
        fflush(stdout);

        /*
         * Using execlp() to invoke bash/sh running an inline loop/seq to print even numbers between 1 to 50.
         * Alternatively, if an external binary like ./print_evens is used, execl("./print_evens", "print_evens", NULL) can be called.
         * Using sh -c ensures it runs directly on any Linux environment without requiring separate binaries!
         */
        execlp("sh", "sh", "-c", "for i in $(seq 2 2 50); do printf \"%d \" \"$i\"; done; echo \"\"", (char *)NULL);

        /* If execlp returns, an error occurred */
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        /* Parent Process */
        int status;
        waitpid(pid, &status, 0);

        printf("\n[Parent PID: %d] Child process execution completed.\n", getpid());
        printf("=====================================================\n");
    }

    return 0;
}
