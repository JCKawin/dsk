/*
 * SCAN (Elevator) Disk Scheduling
 * Compile: gcc scan_disk_scheduling.c -o scan_disk_scheduling
 * Run:     ./scan_disk_scheduling
 *
 * The head starts by moving towards higher cylinders. It services every
 * request on the way, travels to the last cylinder, then reverses and
 * services the remaining requests on the way down.
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

    /* Lower side is serviced on the way down: largest first. */
    for (int i = 0; i < lc / 2; i++) {
        int tmp = lower[i];
        lower[i] = lower[lc - 1 - i];
        lower[lc - 1 - i] = tmp;
    }

    int order[MAX_REQUESTS], oc = 0;
    for (int i = 0; i < uc; i++) order[oc++] = upper[i];
    for (int i = 0; i < lc; i++) order[oc++] = lower[i];

    printf("\nDirection: up (towards higher cylinders), then reverse\n");
    printf("Service order: ");
    for (int i = 0; i < oc; i++) {
        printf("%d", order[i]);
        if (i + 1 < oc) printf(" -> ");
    }
    printf("\n");

    int total = disk_max - head;          /* travel to the end */
    if (lc > 0)
        total += disk_max - lower[lc - 1]; /* sweep back down */

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
Direction: up (towards higher cylinders), then reverse
Service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 37 -> 14
Total head movement: 331 cylinders
Average seek distance: 41.38 cylinders/request
*/
