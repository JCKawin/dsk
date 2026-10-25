/*
 * Non Preemptive SJF CPU Scheduling
 * Compile: gcc non_preemptive_sjf.c -o non_preemptive_sjf
 */

#include <stdio.h>

#define MAX 20
#define INF 1000000000

typedef struct {
    int pid, at, bt, ct, tat, wt, done;
} Process;

void printTable(Process p[], int n) {
    printf("\nPID   AT   BT   CT   TAT  WT\n");
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("P%-4d%-5d%-5d%-5d%-5d%-5d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
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

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time  : ", p[i].pid);
        scanf("%d", &p[i].bt);
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
        int idx = -1, min_bt = INF, best_at = INF;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (p[i].bt < min_bt ||
                    (p[i].bt == min_bt && p[i].at < best_at) ||
                    (p[i].bt == min_bt && p[i].at == best_at && p[i].pid < (idx >= 0 ? p[idx].pid : INF))) {
                    min_bt = p[i].bt;
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
0 8
1 4
2 9
3 5

Sample Output:
Gantt Chart:
| P1 | P2 | P4 | P3 |
0     8    12    17    26

PID   AT   BT   CT   TAT  WT
P1    0    8    8    8    0
P2    1    4    12   11   7
P4    3    5    17   14   9
P3    2    9    26   24   15

Average Waiting Time     = 7.75
Average Turnaround Time  = 14.25
*/
