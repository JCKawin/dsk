/*
 * FCFS Disk Scheduling
 * Compile: gcc fcfs_disk_scheduling.c -o fcfs_disk_scheduling
 * Run:     ./fcfs_disk_scheduling
 *
 * Services requests in the order they arrive, starting from the initial
 * head position.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 100

int main(void) {
    int n, head, req[MAX_REQUESTS];

    printf("Number of requests: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_REQUESTS) {
        printf("Invalid number of requests.\n");
        return 1;
    }

    printf("Enter %d request cylinders:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &req[i]) != 1 || req[i] < 0) {
            printf("Invalid request.\n");
            return 1;
        }
    }

    printf("Initial head position: ");
    if (scanf("%d", &head) != 1 || head < 0) {
        printf("Invalid head position.\n");
        return 1;
    }

    int total = 0;
    int current = head;

    printf("\nService order: ");
    for (int i = 0; i < n; i++) {
        printf("%d", req[i]);
        if (i + 1 < n) printf(" -> ");
        total += abs(req[i] - current);
        current = req[i];
    }

    printf("\nTotal head movement: %d cylinders\n", total);
    printf("Average seek distance: %.2f cylinders/request\n",
           (double)total / n);
    return 0;
}

/*
Sample Input:
8
98 183 37 122 14 124 65 67
53

Sample Output:
Service order: 98 -> 183 -> 37 -> 122 -> 14 -> 124 -> 65 -> 67
Total head movement: 640 cylinders
Average seek distance: 80.00 cylinders/request
*/
