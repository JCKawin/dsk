/*
 * Best Fit Memory Allocation
 * Compile: gcc best_fit.c -o best_fit
 */

#include <stdio.h>

#define MAX 20

typedef struct {
    int id, size, free, pid, psize;
} Block;

int main(void) {
    int nb, nr;
    Block b[MAX];
    int req_pid[MAX], req_size[MAX];

    printf("Number of memory blocks: ");
    if (scanf("%d", &nb) != 1 || nb < 1 || nb > MAX) return 1;

    printf("Enter block sizes:\n");
    for (int i = 0; i < nb; i++) {
        b[i].id = i + 1;
        b[i].free = 1;
        b[i].pid = 0;
        b[i].psize = 0;
        printf("Block %d size: ", i + 1);
        if (scanf("%d", &b[i].size) != 1 || b[i].size <= 0) return 1;
    }

    printf("Number of processes: ");
    if (scanf("%d", &nr) != 1 || nr < 1 || nr > MAX) return 1;

    printf("Enter process sizes:\n");
    for (int i = 0; i < nr; i++) {
        req_pid[i] = i + 1;
        printf("P%d size: ", i + 1);
        if (scanf("%d", &req_size[i]) != 1 || req_size[i] <= 0) return 1;
    }

    for (int i = 0; i < nr; i++) {
        int best = -1;
        for (int j = 0; j < nb; j++) {
            if (b[j].free && b[j].size >= req_size[i]) {
                if (best == -1 || b[j].size < b[best].size)
                    best = j;
            }
        }
        if (best != -1) {
            b[best].free = 0;
            b[best].pid = req_pid[i];
            b[best].psize = req_size[i];
            printf("P%d (%d) -> block %d (size %d)\n", req_pid[i], req_size[i], b[best].id, b[best].size);
        } else {
            printf("P%d (%d) -> cannot be allocated\n", req_pid[i], req_size[i]);
        }
    }

    int internal = 0, external = 0;
    printf("\n%-8s %-10s %-10s %-10s %-12s\n", "Block", "Size", "Status", "Process", "InternalFrag");
    for (int i = 0; i < nb; i++) {
        if (b[i].free) {
            external += b[i].size;
            printf("%-8d %-10d %-10s %-10s %-12s\n", b[i].id, b[i].size, "Free", "-", "-");
        } else {
            int frag = b[i].size - b[i].psize;
            internal += frag;
            printf("%-8d %-10d %-10s P%-9d %-12d\n", b[i].id, b[i].size, "Used", b[i].pid, frag);
        }
    }

    printf("Total internal fragmentation: %d\n", internal);
    printf("Total external fragmentation: %d\n", external);

    return 0;
}

/*
Sample Input:
5
100 500 200 300 600
4
212 417 112 426

Sample Output:
P1 (212) -> block 4 (size 300)
P2 (417) -> block 2 (size 500)
P3 (112) -> block 3 (size 200)
P4 (426) -> block 5 (size 600)

Block    Size       Status     Process    InternalFrag
1        100        Free       -          -           
2        500        Used       P2         83          
3        200        Used       P3         88          
4        300        Used       P1         88          
5        600        Used       P4         174         
Total internal fragmentation: 433
Total external fragmentation: 100
*/
