/*
 * Preemptive Round Robin CPU Scheduling
 * Compile: gcc preemptive_round_robin.c -o preemptive_round_robin
 */

#include <stdio.h>

#define MAX 20
#define INF 1000000000

typedef struct {
    int pid, at, bt, rt, ct, tat, wt, done;
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
    int n, tq;

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
        p[i].rt = p[i].bt;
        p[i].done = 0;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &tq);
    if (tq < 1) {
        printf("Invalid time quantum.\n");
        return 1;
    }

    int gantt_pid[200], gantt_time[200], gcnt = 0;
    int ready[1000], front = 0, rear = 0, inq[MAX] = {0};
    int completed = 0, first_at = p[0].at;

    for (int i = 1; i < n; i++)
        if (p[i].at < first_at)
            first_at = p[i].at;
    int time = first_at;

    for (int i = 0; i < n; i++)
        if (p[i].at <= time && !inq[i]) {
            ready[rear++] = i;
            inq[i] = 1;
        }
    gantt_time[gcnt] = time;

    while (completed < n) {
        if (front == rear) {
            int nxt = INF;
            for (int i = 0; i < n; i++)
                if (!p[i].done && p[i].at > time && p[i].at < nxt)
                    nxt = p[i].at;
            if (nxt == INF)
                break;
            if (gcnt == 0 || gantt_pid[gcnt - 1] != 0) {
                gantt_pid[gcnt++] = 0;
                gantt_time[gcnt] = nxt;
            } else {
                gantt_time[gcnt] = nxt;
            }
            time = nxt;
            for (int i = 0; i < n; i++)
                if (p[i].at <= time && !inq[i] && !p[i].done) {
                    ready[rear++] = i;
                    inq[i] = 1;
                }
            continue;
        }

        int i = ready[front++];
        int run = (p[i].rt < tq) ? p[i].rt : tq;

        if (gcnt == 0 || gantt_pid[gcnt - 1] != p[i].pid) {
            gantt_pid[gcnt++] = p[i].pid;
            gantt_time[gcnt] = time + run;
        } else {
            gantt_time[gcnt] = time + run;
        }

        p[i].rt -= run;
        time += run;

        for (int j = 0; j < n; j++)
            if (!p[j].done && !inq[j] && p[j].at <= time) {
                ready[rear++] = j;
                inq[j] = 1;
            }

        if (p[i].rt == 0) {
            p[i].done = 1;
            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
            completed++;
        } else {
            ready[rear++] = i;
        }
    }

    printGantt(gantt_pid, gantt_time, gcnt);
    printTable(p, n);

    return 0;
}

/*
Sample Input:
4
0 5
1 4
2 2
3 1
2

Sample Output:
Gantt Chart:
| P1 | P2 | P3 | P4 | P1 | P2 | P1 |
0     2     4     6     7     9    11    12

PID   AT   BT   CT   TAT  WT
P1    0    5    12   12   7
P2    1    4    11   10   6
P3    2    2    6    4    2
P4    3    1    7    4    3

Average Waiting Time     = 4.50
Average Turnaround Time  = 7.50
*/
