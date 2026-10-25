/*
 * C-SCAN (Circular SCAN) Disk Scheduling
 * Compile: gcc cscan_disk_scheduling.c -o cscan_disk_scheduling
 * Run:     ./cscan_disk_scheduling
 *
 * The head moves towards higher cylinders, services requests up to the last
 * cylinder, jumps back to cylinder 0 (the return jump is not counted as seek
 * time), then moves up again servicing the remaining requests.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 100

static void sort_ascending(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int main(void) {
    int n, head, disk_max, req[MAX_REQUESTS];

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

    printf("Highest cylinder number: ");
    if (scanf("%d", &disk_max) != 1 || disk_max < 1 || head > disk_max) {
        printf("Invalid disk size.\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (req[i] > disk_max) {
            printf("Request %d is outside the disk.\n", req[i]);
            return 1;
        }
    }

    sort_ascending(req, n);

    int upper[MAX_REQUESTS], lower[MAX_REQUESTS];
    int uc = 0, lc = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] >= head)
            upper[uc++] = req[i];
        else
            lower[lc++] = req[i];
    }

    /* Both groups are serviced upwards; the jump itself is not counted. */
    int order[MAX_REQUESTS], oc = 0;
    for (int i = 0; i < uc; i++) order[oc++] = upper[i];
    for (int i = 0; i < lc; i++) order[oc++] = lower[i];

    printf("\nDirection: up, jump to 0, then up again\n");
    printf("Service order: ");
    for (int i = 0; i < oc; i++) {
        printf("%d", order[i]);
        if (i + 1 < oc) printf(" -> ");
    }
    printf("\n");

    int total = disk_max - head;          /* head to the end */
    if (lc > 0)
        total += disk_max + lower[lc - 1]; /* jump 0->..., then 0->last lower */

    printf("Total head movement: %d cylinders\n", total);
    printf("Average seek distance: %.2f cylinders/request\n",
           (double)total / n);
    return 0;
}

/*
Sample Input:
8
98 183 37 122 14 124 65 67
53
199

Sample Output:
Direction: up, jump to 0, then up again
Service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 14 -> 37
Total head movement: 382 cylinders
Average seek distance: 47.75 cylinders/request
*/
