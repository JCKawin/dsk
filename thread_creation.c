/*
 * Thread Creation using POSIX Threads (pthreads).
 * Demonstrates:
 *   - Creating multiple threads with pthread_create()
 *   - Passing parameters/arguments to thread routines
 *   - Querying thread IDs using pthread_self()
 *   - Shared memory space across threads
 *   - Waiting for threads to finish using pthread_join()
 *   - Returning status/data from threads
 *
 * Compile: gcc thread_creation.c -o thread_creation -pthread
 * Run:     ./thread_creation
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

/* Structure to pass multiple arguments to thread function */
typedef struct {
    int thread_num;
    int work_units;
} thread_data_t;

/* Shared resource among all threads */
static int shared_counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Thread routine */
static void *thread_worker(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int id = data->thread_num;
    pthread_t tid = pthread_self();

    printf("[Thread %d] Created. TID: %lu, Target work units: %d\n", id, (unsigned long)tid, data->work_units);

    /* Simulate work and safely access shared memory */
    for (int i = 1; i <= data->work_units; i++) {
        pthread_mutex_lock(&lock);
        shared_counter++;
        printf("[Thread %d] Step %d/%d (Shared Counter = %d)\n", 
               id, i, data->work_units, shared_counter);
        pthread_mutex_unlock(&lock);
        usleep(50000); /* 50ms delay to demonstrate interleaving */
    }

    /* Allocate result dynamically to return to pthread_join */
    int *result = malloc(sizeof(int));
    if (result != NULL) {
        *result = data->work_units * 10;
    }

    printf("[Thread %d] Completed work. Exiting.\n", id);
    pthread_exit((void *)result);
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_args[NUM_THREADS];
    int rc;

    printf("=========================================\n");
    printf("         Thread Creation Demo            \n");
    printf("=========================================\n");
    printf("[Main] Creating %d POSIX threads...\n\n", NUM_THREADS);

    /* Create threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_num = i + 1;
        thread_args[i].work_units = 3;

        printf("[Main] Spawning Thread %d...\n", i + 1);
        rc = pthread_create(&threads[i], NULL, thread_worker, (void *)&thread_args[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d: return code %d\n", i + 1, rc);
            exit(EXIT_FAILURE);
        }
    }

    printf("\n[Main] All threads spawned. Waiting for them to complete...\n\n");

    /* Wait for threads to complete using pthread_join */
    for (int i = 0; i < NUM_THREADS; i++) {
        void *status;
        rc = pthread_join(threads[i], &status);
        if (rc != 0) {
            fprintf(stderr, "Error joining thread %d: return code %d\n", i + 1, rc);
            exit(EXIT_FAILURE);
        }

        if (status != NULL) {
            int ret_val = *(int *)status;
            printf("[Main] Joined Thread %d (returned value: %d)\n", i + 1, ret_val);
            free(status);
        }
    }

    pthread_mutex_destroy(&lock);

    printf("\n[Main] All threads successfully finished.\n");
    printf("[Main] Final Shared Counter value: %d\n", shared_counter);
    printf("=========================================\n");

    return EXIT_SUCCESS;
}
