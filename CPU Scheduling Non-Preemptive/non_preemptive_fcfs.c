/*
 * Non Preemptive FCFS CPU Scheduling
 * Compile: gcc non_preemptive_fcfs.c -o non_preemptive_fcfs
 */

#include <stdio.h>

#define MAX 20

typedef struct {
    int pid, at, bt, ct, tat, wt;
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
    }

    /* Sort by arrival time (tie break by PID) */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at || (p[j].at == p[j + 1].at && p[j].pid > p[j + 1].pid)) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int gantt_pid[200], gantt_time[200], gcnt = 0;
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) {
            gantt_pid[gcnt] = 0;
            gantt_time[gcnt] = time;
            gcnt++;
            time = p[i].at;
        }
        gantt_pid[gcnt] = p[i].pid;
        gantt_time[gcnt] = time;
        gcnt++;
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
    gantt_time[gcnt] = time;

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
| P1 | P2 | P3 | P4 |
0     8    12    21    26

PID   AT   BT   CT   TAT  WT
P1    0    8    8    8    0
P2    1    4    12   11   7
P3    2    9    21   19   10
P4    3    5    26   23   18

Average Waiting Time     = 8.75
Average Turnaround Time  = 15.25
*/
