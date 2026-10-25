/*
 * Non Preemptive Priority CPU Scheduling (larger priority number = higher priority)
 * Compile: gcc non_preemptive_priority.c -o non_preemptive_priority
 */

#include <stdio.h>

#define MAX 20
#define INF 1000000000

typedef struct {
    int pid, at, bt, pr, ct, tat, wt, done;
} Process;

void printTable(Process p[], int n) {
    printf("\nPID   AT   BT   PR   CT   TAT  WT\n");
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("P%-4d%-5d%-5d%-5d%-5d%-5d%-5d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt);
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    printf("\nAverage Waiting Time     = %.2f\n", avg_wt / n);
    printf("Average Turnaround Time  = %.2f\n", avg_tat / n);
}

void printGantt(int gantt_pid[], int gantt_time[], int gcnt) {
    printf("\nGantt Chart:\n|");
    for (int k = 0; k < gcnt; k++)
        printf(gantt_pid[k] == 0 ? " IDLE |" : " P%d |", gantt_pid[k]);
    printf("\n%d", gantt_time[0]);
    for (int k = 0; k < gcnt; k++)
        printf("%6d", gantt_time[k + 1]);
    printf("\n");
}

int main(void) {
    Process p[MAX];
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    if (n < 1 || n > MAX) {
        printf("Invalid.\n");
        return 1;
    }

    printf("Enter Arrival Time, Burst Time and Priority for each process:\n");
    printf("(Larger priority number = higher priority)\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time  : ", p[i].pid);
        scanf("%d", &p[i].bt);
        printf("P%d Priority    : ", p[i].pid);
        scanf("%d", &p[i].pr);
        p[i].done = 0;
    }

    int gantt_pid[200], gantt_time[200], gcnt = 0;
    int completed = 0, time = 0;

    int min_at = INF;
    for (int i = 0; i < n; i++)
        if (p[i].at < min_at) min_at = p[i].at;
    time = min_at;
    gantt_time[0] = time;

    while (completed < n) {
        int idx = -1, best_pr = -1, best_at = INF;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (p[i].pr > best_pr ||
                    (p[i].pr == best_pr && p[i].at < best_at) ||
                    (p[i].pr == best_pr && p[i].at == best_at && p[i].pid < (idx >= 0 ? p[idx].pid : INF))) {
                    best_pr = p[i].pr;
                    best_at = p[i].at;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            int nxt = INF;
            for (int i = 0; i < n; i++)
                if (!p[i].done && p[i].at > time && p[i].at < nxt) nxt = p[i].at;
            if (nxt == INF) break;
            gantt_pid[gcnt++] = 0;
            time = nxt;
            gantt_time[gcnt] = time;
            continue;
        }

        gantt_pid[gcnt++] = p[idx].pid;
        time += p[idx].bt;
        gantt_time[gcnt] = time;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = 1;
        completed++;
    }

    printGantt(gantt_pid, gantt_time, gcnt);
    printTable(p, n);
    return 0;
}

/*
Sample Input:
4
0 4 1
1 3 2
2 1 4
3 5 3

Sample Output:
Gantt Chart:
| P1 | P3 | P4 | P2 |
0     4     5    10    13

PID   AT   BT   PR   CT   TAT  WT
P1    0    4    1    4    4    0
P3    2    1    4    5    3    2
P4    3    5    3    10   7    2
P2    1    3    2    13   12   9

Average Waiting Time     = 3.25
Average Turnaround Time  = 6.50
*/
