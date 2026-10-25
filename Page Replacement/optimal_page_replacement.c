/*
 * Optimal Page Replacement
 * Compile: gcc optimal_page_replacement.c -o optimal_page_replacement
 */

#include <stdio.h>
#include <limits.h>

#define MAX_REF 50
#define MAX_FRAMES 10

void print_frames(int frames[], int nf) {
    printf("[ ");
    for (int i = 0; i < nf; i++) {
        if (frames[i] == -1) printf("- ");
        else printf("%d ", frames[i]);
    }
    printf("]");
}

int find_page(int frames[], int nf, int page) {
    for (int i = 0; i < nf; i++)
        if (frames[i] == page) return i;
    return -1;
}

int main(void) {
    int n, nf, ref[MAX_REF];
    printf("Number of frames: ");
    scanf("%d", &nf);
    if (nf < 1 || nf > MAX_FRAMES) return 1;
    printf("Length of reference string: ");
    scanf("%d", &n);
    if (n < 1 || n > MAX_REF) return 1;
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &ref[i]);

    int frames[MAX_FRAMES];
    for (int i = 0; i < nf; i++) frames[i] = -1;
    int faults = 0, filled = 0;

    printf("\n%-8s %-20s %s\n", "Page", "Frames", "Result");
    for (int i = 0; i < n; i++) {
        int idx = find_page(frames, nf, ref[i]);
        if (idx != -1) {
            printf("%-8d ", ref[i]);
            print_frames(frames, nf);
            printf(" Hit\n");
        } else {
            int victim;
            if (filled < nf) {
                victim = filled++;
            } else {
                int farthest = -1;
                victim = 0;
                for (int f = 0; f < nf; f++) {
                    int next_use = INT_MAX;
                    for (int k = i + 1; k < n; k++) {
                        if (ref[k] == frames[f]) {
                            next_use = k;
                            break;
                        }
                    }
                    if (next_use > farthest) {
                        farthest = next_use;
                        victim = f;
                    }
                }
            }
            frames[victim] = ref[i];
            faults++;
            printf("%-8d ", ref[i]);
            print_frames(frames, nf);
            printf(" Fault\n");
        }
    }
    printf("Page faults: %d / %d    Hit ratio: %.2f\n",
           faults, n, (double)(n - faults) / n);
    return 0;
}

/*
Sample Input:
3
12
7 0 1 2 0 3 0 4 2 3 0 3

Sample Output:
Page     Frames               Result
7        [ 7 - - ]            Fault
0        [ 7 0 - ]            Fault
1        [ 7 0 1 ]            Fault
2        [ 2 0 1 ]            Fault
0        [ 2 0 1 ]            Hit
3        [ 2 0 3 ]            Fault
0        [ 2 0 3 ]            Hit
4        [ 2 4 3 ]            Fault
2        [ 2 4 3 ]            Hit
3        [ 2 4 3 ]            Hit
0        [ 0 4 3 ]            Fault
3        [ 0 4 3 ]            Hit
Page faults: 7 / 12    Hit ratio: 0.42
*/
