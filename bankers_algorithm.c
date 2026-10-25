/*
 * Banker's Algorithm for Deadlock Avoidance
 * Compile: gcc bankers_algorithm.c -o bankers
 *
 * Reads Allocation, Max, and Available.
 * Need = Max - Allocation.
 * Checks whether the current state is safe, and optionally
 * whether a resource request can be granted immediately.
 */

#include <stdio.h>
#include <string.h>

#define MAX_P 10
#define MAX_R 10

static int n, m;
static int allocation[MAX_P][MAX_R];
static int maximum[MAX_P][MAX_R];
static int need[MAX_P][MAX_R];
static int available[MAX_R];

static void compute_need(void) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

static void print_matrices(void) {
    printf("\n%-8s", "Process");
    printf("%-18s %-18s %-18s\n", "Allocation", "Max", "Need");
    for (int i = 0; i < n; i++) {
        printf("P%-7d", i);
        for (int j = 0; j < m; j++) printf("%d ", allocation[i][j]);
        printf("%*s", (int)(18 - 2 * m), "");
        for (int j = 0; j < m; j++) printf("%d ", maximum[i][j]);
        printf("%*s", (int)(18 - 2 * m), "");
        for (int j = 0; j < m; j++) printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", available[j]);
    printf("\n");
}

/*
 * Safety algorithm.
 * work[] starts as Available.
 * A process can run if Need[i] <= work.
 * On finish, its Allocation is added back to work.
 * Returns 1 and fills safe[] if a safe sequence exists.
 */
static int is_safe(int safe[]) {
    int work[MAX_R];
    int finish[MAX_P] = {0};
    int count = 0;

    memcpy(work, available, sizeof(int) * m);

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i]) continue;
            int can = 1;
            for (int j = 0; j < m; j++) {
                if (need[i][j] > work[j]) {
                    can = 0;
                    break;
                }
            }
            if (can) {
                for (int j = 0; j < m; j++)
                    work[j] += allocation[i][j];
                safe[count++] = i;
                finish[i] = 1;
                found = 1;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

static void print_safe(int safe[]) {
    printf("System is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("P%d%s", safe[i], i + 1 == n ? "\n" : " -> ");
    }
}

/* Resource-request algorithm for process pid. */
static void handle_request(int pid, int request[]) {
    printf("\nRequest from P%d: ", pid);
    for (int j = 0; j < m; j++) printf("%d ", request[j]);
    printf("\n");

    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: request exceeds Need. Denied.\n");
            return;
        }
        if (request[j] > available[j]) {
            printf("Process must wait: not enough resources available.\n");
            return;
        }
    }

    /* Pretend to allocate */
    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    int safe[MAX_P];
    if (is_safe(safe)) {
        printf("Request can be granted immediately.\n");
        print_safe(safe);
    } else {
        printf("Request would lead to an UNSAFE state. Rolled back.\n");
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
    }
}

int main(void) {
    printf("Number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_P) return 1;
    printf("Number of resource types: ");
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_R) return 1;

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (scanf("%d", &allocation[i][j]) != 1) return 1;

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (scanf("%d", &maximum[i][j]) != 1) return 1;
            if (maximum[i][j] < allocation[i][j]) {
                fprintf(stderr, "Max cannot be less than Allocation\n");
                return 1;
            }
        }

    printf("Enter Available vector (%d values): ", m);
    for (int j = 0; j < m; j++)
        if (scanf("%d", &available[j]) != 1) return 1;

    compute_need();
    print_matrices();

    int safe[MAX_P];
    printf("\n--- Safety check ---\n");
    if (is_safe(safe)) print_safe(safe);
    else printf("System is in an UNSAFE state. Deadlock possible.\n");

    char again = 'y';
    printf("\nCheck a resource request? (y/n): ");
    if (scanf(" %c", &again) == 1 && (again == 'y' || again == 'Y')) {
        int pid, request[MAX_R];
        printf("Process id (0..%d): ", n - 1);
        if (scanf("%d", &pid) != 1 || pid < 0 || pid >= n) return 1;
        printf("Request vector (%d values): ", m);
        for (int j = 0; j < m; j++)
            if (scanf("%d", &request[j]) != 1) return 1;
        handle_request(pid, request);
    }
    return 0;
}

/*
Sample input (classic textbook example):
5
3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
3 3 2
y
1
1 0 2

Expected: safe sequence exists (e.g. P1 P3 P4 P0 P2),
and P1 request (1 0 2) can be granted.
*/
