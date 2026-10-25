/*
 * LOOK Disk Scheduling
 * Compile: gcc look_disk_scheduling.c -o look_disk_scheduling
 * Run:     ./look_disk_scheduling
 *
 * Like SCAN, but the head reverses at the last pending request instead of
 * travelling all the way to the end of the disk.
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

    sort_ascending(req, n);

    int upper[MAX_REQUESTS], lower[MAX_REQUESTS];
    int uc = 0, lc = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] >= head)
            upper[uc++] = req[i];
        else
            lower[lc++] = req[i];
    }

    /* Lower side is serviced downwards: largest first. */
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

    int total;
    if (uc > 0) {
        total = upper[uc - 1] - head;           /* to highest request */
        if (lc > 0)
            total += upper[uc - 1] - lower[lc - 1]; /* down to lowest */
    } else {
        total = head - lower[lc - 1];           /* no request above */
    }

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

Sample Output:
Direction: up (towards higher cylinders), then reverse
Service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 37 -> 14
Total head movement: 299 cylinders
Average seek distance: 37.38 cylinders/request
*/
