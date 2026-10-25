/*
 * SSTF (Shortest Seek Time First) Disk Scheduling
 * Compile: gcc sstf_disk_scheduling.c -o sstf_disk_scheduling
 * Run:     ./sstf_disk_scheduling
 *
 * Always services the pending request closest to the current head position.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_REQUESTS 100

int main(void) {
    int n, head, req[MAX_REQUESTS];
    int served[MAX_REQUESTS] = {0};

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
    for (int step = 0; step < n; step++) {
        int best = -1;
        int best_dist = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (served[i]) continue;
            int dist = abs(req[i] - current);
            if (dist < best_dist) {
                best_dist = dist;
                best = i;
            }
        }

        printf("%d", req[best]);
        if (step + 1 < n) printf(" -> ");

        total += best_dist;
        current = req[best];
        served[best] = 1;
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
Service order: 65 -> 67 -> 37 -> 14 -> 98 -> 122 -> 124 -> 183
Total head movement: 236 cylinders
Average seek distance: 29.50 cylinders/request
*/
