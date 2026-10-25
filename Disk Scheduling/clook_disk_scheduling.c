/*
 * C-LOOK (Circular LOOK) Disk Scheduling
 * Compile: gcc clook_disk_scheduling.c -o clook_disk_scheduling
 * Run:     ./clook_disk_scheduling
 *
 * Like C-SCAN, but the head jumps from the highest pending request to the
 * lowest pending request instead of travelling to the disk boundaries. The
 * return jump is not counted as seek time.
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

    /* Both groups are serviced upwards; the circular jump is not counted. */
    int order[MAX_REQUESTS], oc = 0;
    for (int i = 0; i < uc; i++) order[oc++] = upper[i];
    for (int i = 0; i < lc; i++) order[oc++] = lower[i];

    printf("\nDirection: up, jump to lowest pending request, then up again\n");
    printf("Service order: ");
    for (int i = 0; i < oc; i++) {
        printf("%d", order[i]);
        if (i + 1 < oc) printf(" -> ");
    }
    printf("\n");

    int total = 0;
    if (uc > 0)
        total += upper[uc - 1] - head;   /* to highest request */
    if (lc > 0)
        total += lower[lc - 1] - lower[0]; /* lowest -> highest lower */

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
Direction: up, jump to lowest pending request, then up again
Service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 14 -> 37
Total head movement: 153 cylinders
Average seek distance: 19.12 cylinders/request
*/
