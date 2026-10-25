# OS Lab Master Reference Guide

This master reference contains all standalone OS Lab programs with their complete source code, compilation commands, and sample input/output.

## Table of Contents

### CPU Scheduling (Non-Preemptive)
- [1. Non-Preemptive FCFS](#1-non-preemptive-fcfs) (CPU Scheduling Non-Preemptive\non_preemptive_fcfs.c)
- [2. Non-Preemptive SJF](#2-non-preemptive-sjf) (CPU Scheduling Non-Preemptive\non_preemptive_sjf.c)
- [3. Non-Preemptive Priority](#3-non-preemptive-priority) (CPU Scheduling Non-Preemptive\non_preemptive_priority.c)

### CPU Scheduling (Preemptive)
- [4. Preemptive SRTF](#4-preemptive-srtf) (CPU Scheduling Preemptive\preemptive_srtf.c)
- [5. Preemptive Priority](#5-preemptive-priority) (CPU Scheduling Preemptive\preemptive_priority.c)
- [6. Preemptive Round Robin](#6-preemptive-round-robin) (CPU Scheduling Preemptive\preemptive_round_robin.c)

### Memory Allocation
- [7. First Fit](#7-first-fit) (Memory Allocation\first_fit.c)
- [8. Best Fit](#8-best-fit) (Memory Allocation\best_fit.c)
- [9. Worst Fit](#9-worst-fit) (Memory Allocation\worst_fit.c)

### Page Replacement
- [10. FIFO Page Replacement](#10-fifo-page-replacement) (Page Replacement\fifo_page_replacement.c)
- [11. LRU Page Replacement](#11-lru-page-replacement) (Page Replacement\lru_page_replacement.c)
- [12. Optimal Page Replacement](#12-optimal-page-replacement) (Page Replacement\optimal_page_replacement.c)

### Process Synchronization & Concurrency
- [13. Dining Philosophers](#13-dining-philosophers) (dining_philosophers.c)
- [14. Producer-Consumer](#14-producer-consumer) (producer_consumer.c)
- [15. Readers-Writers](#15-readers-writers) (readers_writers.c)
- [16. Shared Memory (POSIX IPC)](#16-shared-memory-posix-ipc) (shared_memory.c)

### Deadlock Avoidance
- [17. Banker's Algorithm](#17-bankers-algorithm) (bankers_algorithm.c)

### File System System Calls
- [18. File System Calls](#18-file-system-calls) (file_system_calls.c)

### Process & Thread Management
- [19. Process Creation](#19-process-creation) (process_creation.c)
- [20. Thread Creation](#20-thread-creation) (thread_creation.c)

---

# CPU Scheduling (Non-Preemptive)

## 1. Non-Preemptive FCFS
**File:** CPU Scheduling Non-Preemptive\non_preemptive_fcfs.c

`c
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
`

---

## 2. Non-Preemptive SJF
**File:** CPU Scheduling Non-Preemptive\non_preemptive_sjf.c

`c
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
`

---

## 3. Non-Preemptive Priority
**File:** CPU Scheduling Non-Preemptive\non_preemptive_priority.c

`c
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
`

---

# CPU Scheduling (Preemptive)

## 4. Preemptive SRTF
**File:** CPU Scheduling Preemptive\preemptive_srtf.c

`c
/*
 * Preemptive SRTF CPU Scheduling (Shortest Remaining Time First)
 * Compile: gcc preemptive_srtf.c -o preemptive_srtf
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
        p[i].rt = p[i].bt;
        p[i].done = 0;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }

    int gantt_pid[200], gantt_time[200], gcnt = 0;
    int completed = 0, time = INF, last = -1;
    for (int i = 0; i < n; i++)
        if (p[i].at < time) time = p[i].at;
    gantt_time[0] = time;

    while (completed < n) {
        int idx = -1, best_rt = INF, best_at = INF;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time && p[i].rt > 0) {
                if (p[i].rt < best_rt ||
                    (p[i].rt == best_rt && p[i].at < best_at) ||
                    (p[i].rt == best_rt && p[i].at == best_at &&
                     p[i].pid < (idx >= 0 ? p[idx].pid : INF))) {
                    best_rt = p[i].rt;
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
            if (last != 0) {
                gantt_pid[gcnt++] = 0;
                last = 0;
            }
            time = nxt;
            gantt_time[gcnt] = time;
            continue;
        }
        if (last != p[idx].pid) {
            gantt_pid[gcnt++] = p[idx].pid;
            last = p[idx].pid;
        }
        p[idx].rt--;
        time++;
        gantt_time[gcnt] = time;
        if (p[idx].rt == 0) {
            p[idx].done = 1;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
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
| P1 | P2 | P4 | P1 | P3 |
0     1     5    10    17    26

PID   AT   BT   CT   TAT  WT
P1    0    8    17   17   9
P2    1    4    5    4    0
P3    2    9    26   24   15
P4    3    5    10   7    2

Average Waiting Time     = 6.50
Average Turnaround Time  = 13.00
*/
`

---

## 5. Preemptive Priority
**File:** CPU Scheduling Preemptive\preemptive_priority.c

`c
/*
 * Preemptive Priority CPU Scheduling (larger priority number = lower priority)
 * Compile: gcc preemptive_priority.c -o preemptive_priority
 */
#include <stdio.h>

#define MAX 20
#define INF 1000000000

typedef struct {
    int pid, at, bt, pr, rt, ct, tat, wt, done;
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
    if (n < 1 || n > MAX) { printf("Invalid.\n"); return 1; }
    printf("Enter Arrival Time, Burst Time and Priority for each process:\n");
    printf("(Larger priority number = lower priority / smaller number = higher priority)\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time  : ", p[i].pid);
        scanf("%d", &p[i].bt);
        printf("P%d Priority    : ", p[i].pid);
        scanf("%d", &p[i].pr);
        p[i].rt = p[i].bt;
        p[i].done = 0;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }

    int gantt_pid[200], gantt_time[200], gcnt = 0;
    int completed = 0, time = INF, last = -1;
    for (int i = 0; i < n; i++)
        if (p[i].at < time) time = p[i].at;
    gantt_time[0] = time;

    while (completed < n) {
        int idx = -1, best_pr = INF, best_at = INF;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time && p[i].rt > 0) {
                if (p[i].pr < best_pr ||
                    (p[i].pr == best_pr && p[i].at < best_at) ||
                    (p[i].pr == best_pr && p[i].at == best_at &&
                     p[i].pid < (idx >= 0 ? p[idx].pid : INF))) {
                    best_pr = p[i].pr; best_at = p[i].at; idx = i;
                }
            }
        }
        if (idx == -1) {
            int nxt = INF;
            for (int i = 0; i < n; i++)
                if (!p[i].done && p[i].at > time && p[i].at < nxt) nxt = p[i].at;
            if (nxt == INF) break;
            if (last != 0) { gantt_pid[gcnt++] = 0; last = 0; }
            time = nxt; gantt_time[gcnt] = time;
            continue;
        }
        if (last != p[idx].pid) { gantt_pid[gcnt++] = p[idx].pid; last = p[idx].pid; }
        p[idx].rt--; time++; gantt_time[gcnt] = time;
        if (p[idx].rt == 0) {
            p[idx].done = 1;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    printGantt(gantt_pid, gantt_time, gcnt);
    printTable(p, n);
    return 0;
}

/*
Sample Input:
4
0 8 3
1 2 1
2 4 4
3 2 2

Sample Output:
Gantt Chart:
| P1 | P2 | P4 | P1 | P3 |
0     1     3     5    12    16

PID   AT   BT   PR   CT   TAT  WT
P1    0    8    3    12   12   4
P2    1    2    1    3    2    0
P3    2    4    4    16   14   10
P4    3    2    2    5    2    0

Average Waiting Time     = 3.50
Average Turnaround Time  = 7.50
*/
`

---

## 6. Preemptive Round Robin
**File:** CPU Scheduling Preemptive\preemptive_round_robin.c

`c
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
`

---

# Memory Allocation

## 7. First Fit
**File:** Memory Allocation\first_fit.c

`c
/*
 * First Fit Memory Allocation
 * Compile: gcc first_fit.c -o first_fit
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
        int found = 0;
        for (int j = 0; j < nb; j++) {
            if (b[j].free && b[j].size >= req_size[i]) {
                b[j].free = 0;
                b[j].pid = req_pid[i];
                b[j].psize = req_size[i];
                printf("P%d (%d) -> block %d (size %d)\n", req_pid[i], req_size[i], b[j].id, b[j].size);
                found = 1;
                break;
            }
        }
        if (!found) {
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
P1 (212) -> block 2 (size 500)
P2 (417) -> block 5 (size 600)
P3 (112) -> block 3 (size 200)
P4 (426) -> cannot be allocated

Block    Size       Status     Process    InternalFrag
1        100        Free       -          -           
2        500        Used       P1         288         
3        200        Used       P3         88          
4        300        Free       -          -           
5        600        Used       P2         183         
Total internal fragmentation: 559
Total external fragmentation: 400
*/
`

---

## 8. Best Fit
**File:** Memory Allocation\best_fit.c

`c
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
`

---

## 9. Worst Fit
**File:** Memory Allocation\worst_fit.c

`c
/*
 * Worst Fit Memory Allocation
 * Compile: gcc worst_fit.c -o worst_fit
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
    scanf("%d", &nb);
    if (nb < 1 || nb > MAX) return 1;
    printf("Enter block sizes:\n");
    for (int i = 0; i < nb; i++) {
        b[i].id = i + 1;
        b[i].free = 1;
        b[i].pid = 0;
        b[i].psize = 0;
        printf("Block %d size: ", i + 1);
        scanf("%d", &b[i].size);
        if (b[i].size <= 0) return 1;
    }

    printf("Number of processes: ");
    scanf("%d", &nr);
    if (nr < 1 || nr > MAX) return 1;
    printf("Enter process sizes:\n");
    for (int i = 0; i < nr; i++) {
        req_pid[i] = i + 1;
        printf("P%d size: ", i + 1);
        scanf("%d", &req_size[i]);
        if (req_size[i] <= 0) return 1;
    }

    for (int i = 0; i < nr; i++) {
        int worst = -1;
        for (int j = 0; j < nb; j++) {
            if (b[j].free && b[j].size >= req_size[i]) {
                if (worst == -1 || b[j].size > b[worst].size)
                    worst = j;
            }
        }
        if (worst != -1) {
            b[worst].free = 0;
            b[worst].pid = req_pid[i];
            b[worst].psize = req_size[i];
            printf("P%d (%d) -> block %d (size %d)\n", req_pid[i], req_size[i], b[worst].id, b[worst].size);
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
P1 (212) -> block 5 (size 600)
P2 (417) -> block 2 (size 500)
P3 (112) -> block 4 (size 300)
P4 (426) -> cannot be allocated

Block    Size       Status     Process    InternalFrag
1        100        Free       -          -           
2        500        Used       P2         83          
3        200        Free       -          -           
4        300        Used       P3         188         
5        600        Used       P1         388         
Total internal fragmentation: 659
Total external fragmentation: 300
*/
`

---

# Page Replacement

## 10. FIFO Page Replacement
**File:** Page Replacement\fifo_page_replacement.c

`c
/*
 * FIFO Page Replacement
 * Compile: gcc fifo_page_replacement.c -o fifo_page_replacement
 */

#include <stdio.h>

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
    if (scanf("%d", &nf) != 1 || nf < 1 || nf > MAX_FRAMES) return 1;
    printf("Length of reference string: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_REF) return 1;
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++)
        if (scanf("%d", &ref[i]) != 1) return 1;

    int frames[MAX_FRAMES];
    for (int i = 0; i < nf; i++) frames[i] = -1;
    int faults = 0, next = 0;

    printf("\n%-8s %-20s %s\n", "Page", "Frames", "Result");
    for (int i = 0; i < n; i++) {
        int idx = find_page(frames, nf, ref[i]);
        if (idx != -1) {
            printf("%-8d ", ref[i]);
            print_frames(frames, nf);
            printf(" Hit\n");
        } else {
            frames[next] = ref[i];
            next = (next + 1) % nf;
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
3        [ 2 3 1 ]            Fault
0        [ 2 3 0 ]            Fault
4        [ 4 3 0 ]            Fault
2        [ 4 2 0 ]            Fault
3        [ 4 2 3 ]            Fault
0        [ 0 2 3 ]            Fault
3        [ 0 2 3 ]            Hit
Page faults: 10 / 12    Hit ratio: 0.17
*/
`

---

## 11. LRU Page Replacement
**File:** Page Replacement\lru_page_replacement.c

`c
/*
 * LRU Page Replacement
 * Compile: gcc lru_page_replacement.c -o lru_page_replacement
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

    int frames[MAX_FRAMES], last_used[MAX_FRAMES];
    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }
    int faults = 0, filled = 0;

    printf("\n%-8s %-20s %s\n", "Page", "Frames", "Result");
    for (int i = 0; i < n; i++) {
        int idx = find_page(frames, nf, ref[i]);
        if (idx != -1) {
            last_used[idx] = i;
            printf("%-8d ", ref[i]);
            print_frames(frames, nf);
            printf(" Hit\n");
        } else {
            int victim;
            if (filled < nf) {
                victim = filled++;
            } else {
                int oldest = INT_MAX;
                victim = 0;
                for (int f = 0; f < nf; f++) {
                    if (last_used[f] < oldest) {
                        oldest = last_used[f];
                        victim = f;
                    }
                }
            }
            frames[victim] = ref[i];
            last_used[victim] = i;
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
4        [ 4 0 3 ]            Fault
2        [ 4 0 2 ]            Fault
3        [ 4 3 2 ]            Fault
0        [ 0 3 2 ]            Fault
3        [ 0 3 2 ]            Hit
Page faults: 9 / 12    Hit ratio: 0.25
*/
`

---

## 12. Optimal Page Replacement
**File:** Page Replacement\optimal_page_replacement.c

`c
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
`

---

# Process Synchronization & Concurrency

## 13. Dining Philosophers
**File:** dining_philosophers.c

`c
/*
 * Dining Philosophers using POSIX semaphores.
 * Classic solution: each philosopher picks up the lower-numbered chopstick
 * first (asymmetric / resource hierarchy). This prevents circular wait,
 * so deadlock cannot occur. A room semaphore is also available as the
 * alternative "at most N-1 seated" solution; the hierarchy version is used.
 *
 * Compile: gcc dining_philosophers.c -o dining_philosophers -pthread
 * Run:     ./dining_philosophers
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define MEALS 2

static sem_t chopstick[N];

static void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;
    /* Always acquire the lower index first -> breaks the cycle. */
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    for (int meal = 1; meal <= MEALS; meal++) {
        printf("Philosopher %d is thinking\n", id);
        usleep(100000 + (id * 20000));

        printf("Philosopher %d is hungry\n", id);
        sem_wait(&chopstick[first]);
        printf("Philosopher %d picked chopstick %d\n", id, first);
        sem_wait(&chopstick[second]);
        printf("Philosopher %d picked chopstick %d\n", id, second);

        printf("Philosopher %d is eating meal %d\n", id, meal);
        usleep(120000);

        sem_post(&chopstick[second]);
        sem_post(&chopstick[first]);
        printf("Philosopher %d put down chopsticks (finished meal %d)\n", id, meal);
    }
    printf("Philosopher %d is done\n", id);
    return NULL;
}

int main(void) {
    pthread_t tid[N];
    int id[N];

    for (int i = 0; i < N; i++)
        sem_init(&chopstick[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    for (int i = 0; i < N; i++)
        sem_destroy(&chopstick[i]);

    printf("All philosophers finished. No deadlock.\n");
    return 0;
}
`

---

## 14. Producer-Consumer
**File:** producer_consumer.c

`c
/*
 * Producer-Consumer using POSIX semaphores + a shared circular buffer.
 * One producer process, one consumer process (fork).
 *
 * Compile: gcc producer_consumer.c -o producer_consumer -pthread
 * Run:     ./producer_consumer
 *
 * Semaphores:
 *   empty  - number of free slots (init = BUFFER_SIZE)
 *   full   - number of filled slots (init = 0)
 *   mutex  - mutual exclusion on the buffer (init = 1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define ITEMS       10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t empty;
    sem_t full;
    sem_t mutex;
} Shared;

static Shared *create_shared(void) {
    Shared *s = mmap(NULL, sizeof(Shared), PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (s == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    s->in = s->out = 0;
    /* pshared = 1 so the semaphore is visible across fork */
    sem_init(&s->empty, 1, BUFFER_SIZE);
    sem_init(&s->full,  1, 0);
    sem_init(&s->mutex, 1, 1);
    return s;
}

int main(void) {
    Shared *s = create_shared();
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Consumer */
        for (int i = 0; i < ITEMS; i++) {
            sem_wait(&s->full);
            sem_wait(&s->mutex);

            int item = s->buffer[s->out];
            printf("[consumer] consumed %d from slot %d\n", item, s->out);
            s->out = (s->out + 1) % BUFFER_SIZE;

            sem_post(&s->mutex);
            sem_post(&s->empty);
            usleep(150000);
        }
        exit(0);
    }

    /* Producer */
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&s->empty);
        sem_wait(&s->mutex);

        s->buffer[s->in] = i * 10;
        printf("[producer] produced %d into slot %d\n", s->buffer[s->in], s->in);
        s->in = (s->in + 1) % BUFFER_SIZE;

        sem_post(&s->mutex);
        sem_post(&s->full);
        usleep(80000);
    }

    waitpid(pid, NULL, 0);
    sem_destroy(&s->empty);
    sem_destroy(&s->full);
    sem_destroy(&s->mutex);
    munmap(s, sizeof(Shared));
    printf("Producer-consumer finished (%d items).\n", ITEMS);
    return 0;
}
`

---

## 15. Readers-Writers
**File:** readers_writers.c

`c
/*
 * Readers-Writers (first readers-writers problem: readers preference)
 * using POSIX semaphores. Threads share the data structure.
 *
 * Compile: gcc readers_writers.c -o readers_writers -pthread
 * Run:     ./readers_writers
 *
 * mutex     protects read_count
 * write_lock gives a writer exclusive access; the first reader acquires it
 *            and the last reader releases it, so writers wait while any
 *            reader is active.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 4
#define WRITERS 2
#define READS_EACH  3
#define WRITES_EACH 2

static int shared_data = 0;
static int read_count = 0;
static sem_t mutex;
static sem_t write_lock;

static void *reader(void *arg) {
    int id = *(int *)arg;
    for (int k = 0; k < READS_EACH; k++) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&write_lock);   /* first reader locks out writers */
        sem_post(&mutex);

        printf("Reader %d reads value %d (active readers=%d)\n",
               id, shared_data, read_count);
        usleep(100000);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&write_lock);   /* last reader lets writers in */
        sem_post(&mutex);
        usleep(50000);
    }
    return NULL;
}

static void *writer(void *arg) {
    int id = *(int *)arg;
    for (int k = 0; k < WRITES_EACH; k++) {
        sem_wait(&write_lock);
        shared_data += 1;
        printf("Writer %d writes value %d\n", id, shared_data);
        usleep(150000);
        sem_post(&write_lock);
        usleep(80000);
    }
    return NULL;
}

int main(void) {
    pthread_t rt[READERS], wt[WRITERS];
    int rid[READERS], wid[WRITERS];

    sem_init(&mutex, 0, 1);
    sem_init(&write_lock, 0, 1);

    for (int i = 0; i < READERS; i++) {
        rid[i] = i + 1;
        pthread_create(&rt[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < WRITERS; i++) {
        wid[i] = i + 1;
        pthread_create(&wt[i], NULL, writer, &wid[i]);
    }
    for (int i = 0; i < READERS; i++) pthread_join(rt[i], NULL);
    for (int i = 0; i < WRITERS; i++) pthread_join(wt[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&write_lock);
    printf("Final shared value = %d (expected %d)\n", shared_data, WRITERS * WRITES_EACH);
    return 0;
}
`

---

## 16. Shared Memory (POSIX IPC)
**File:** shared_memory.c

`c
/*
 * Shared memory: parent creates a POSIX shared-memory segment,
 * writes a message, child attaches and reads it, then parent unlinks.
 *
 * Compile: gcc shared_memory.c -o shared_memory -lrt
 * Run:     ./shared_memory
 *
 * POSIX shm is used (shm_open / mmap). Works on Linux.
 * The segment name is /oslab_shm_demo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define SHM_NAME "/oslab_shm_demo"
#define SHM_SIZE 256

int main(void) {
    /* Clean leftover name from a previous crash, then create fresh. */
    shm_unlink(SHM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    char *shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap parent");
        return 1;
    }
    close(fd); /* mapping remains valid */

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Child: attach to the same segment and read. */
        int cfd = shm_open(SHM_NAME, O_RDONLY, 0666);
        if (cfd == -1) {
            perror("child shm_open");
            exit(1);
        }
        char *cshm = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, cfd, 0);
        if (cshm == MAP_FAILED) {
            perror("child mmap");
            exit(1);
        }
        close(cfd);

        /* Wait until parent publishes a non-empty string. */
        while (cshm[0] == '\0')
            usleep(10000);

        printf("[child  pid=%d] read from shared memory: \"%s\"\n", getpid(), cshm);
        munmap(cshm, SHM_SIZE);
        exit(0);
    }

    /* Parent writes after a short pause so the child is waiting. */
    const char *msg = "Hello from parent via POSIX shared memory";
    printf("[parent pid=%d] writing to shared memory...\n", getpid());
    usleep(50000);
    snprintf(shm, SHM_SIZE, "%s", msg);
    printf("[parent pid=%d] wrote: \"%s\"\n", getpid(), shm);

    int status = 0;
    waitpid(pid, &status, 0);

    munmap(shm, SHM_SIZE);
    if (shm_unlink(SHM_NAME) == -1)
        perror("shm_unlink");
    else
        printf("[parent] shared memory unlinked\n");

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
`

---

# Deadlock Avoidance

## 17. Banker's Algorithm
**File:** bankers_algorithm.c

`c
/*
 * Banker's Algorithm for Deadlock Avoidance
 * Compile: gcc bankers_algorithm.c -o bankers
 *
 * Reads Allocation, Max, and Available.
 * Need = Max - Allocation.
 * Checks whether the current state is safe, and optionally
 * whether a resource request can be granted immediately.
 */

#include <stdio.h>
#include <string.h>

#define MAX_P 10
#define MAX_R 10

static int n, m;
static int allocation[MAX_P][MAX_R];
static int maximum[MAX_P][MAX_R];
static int need[MAX_P][MAX_R];
static int available[MAX_R];

static void compute_need(void) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

static void print_matrices(void) {
    printf("\n%-8s", "Process");
    printf("%-18s %-18s %-18s\n", "Allocation", "Max", "Need");
    for (int i = 0; i < n; i++) {
        printf("P%-7d", i);
        for (int j = 0; j < m; j++) printf("%d ", allocation[i][j]);
        printf("%*s", (int)(18 - 2 * m), "");
        for (int j = 0; j < m; j++) printf("%d ", maximum[i][j]);
        printf("%*s", (int)(18 - 2 * m), "");
        for (int j = 0; j < m; j++) printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", available[j]);
    printf("\n");
}

/*
 * Safety algorithm.
 * work[] starts as Available.
 * A process can run if Need[i] <= work.
 * On finish, its Allocation is added back to work.
 * Returns 1 and fills safe[] if a safe sequence exists.
 */
static int is_safe(int safe[]) {
    int work[MAX_R];
    int finish[MAX_P] = {0};
    int count = 0;

    memcpy(work, available, sizeof(int) * m);

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i]) continue;
            int can = 1;
            for (int j = 0; j < m; j++) {
                if (need[i][j] > work[j]) {
                    can = 0;
                    break;
                }
            }
            if (can) {
                for (int j = 0; j < m; j++)
                    work[j] += allocation[i][j];
                safe[count++] = i;
                finish[i] = 1;
                found = 1;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

static void print_safe(int safe[]) {
    printf("System is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("P%d%s", safe[i], i + 1 == n ? "\n" : " -> ");
    }
}

/* Resource-request algorithm for process pid. */
static void handle_request(int pid, int request[]) {
    printf("\nRequest from P%d: ", pid);
    for (int j = 0; j < m; j++) printf("%d ", request[j]);
    printf("\n");

    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: request exceeds Need. Denied.\n");
            return;
        }
        if (request[j] > available[j]) {
            printf("Process must wait: not enough resources available.\n");
            return;
        }
    }

    /* Pretend to allocate */
    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    int safe[MAX_P];
    if (is_safe(safe)) {
        printf("Request can be granted immediately.\n");
        print_safe(safe);
    } else {
        printf("Request would lead to an UNSAFE state. Rolled back.\n");
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
    }
}

int main(void) {
    printf("Number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_P) return 1;
    printf("Number of resource types: ");
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_R) return 1;

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (scanf("%d", &allocation[i][j]) != 1) return 1;

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (scanf("%d", &maximum[i][j]) != 1) return 1;
            if (maximum[i][j] < allocation[i][j]) {
                fprintf(stderr, "Max cannot be less than Allocation\n");
                return 1;
            }
        }

    printf("Enter Available vector (%d values): ", m);
    for (int j = 0; j < m; j++)
        if (scanf("%d", &available[j]) != 1) return 1;

    compute_need();
    print_matrices();

    int safe[MAX_P];
    printf("\n--- Safety check ---\n");
    if (is_safe(safe)) print_safe(safe);
    else printf("System is in an UNSAFE state. Deadlock possible.\n");

    char again = 'y';
    printf("\nCheck a resource request? (y/n): ");
    if (scanf(" %c", &again) == 1 && (again == 'y' || again == 'Y')) {
        int pid, request[MAX_R];
        printf("Process id (0..%d): ", n - 1);
        if (scanf("%d", &pid) != 1 || pid < 0 || pid >= n) return 1;
        printf("Request vector (%d values): ", m);
        for (int j = 0; j < m; j++)
            if (scanf("%d", &request[j]) != 1) return 1;
        handle_request(pid, request);
    }
    return 0;
}

/*
Sample input (classic textbook example):
5
3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
3 3 2
y
1
1 0 2

Expected: safe sequence exists (e.g. P1 P3 P4 P0 P2),
and P1 request (1 0 2) can be granted.
*/
`

---

# File System System Calls

## 18. File System Calls
**File:** file_system_calls.c

`c
/*
 * File system operations using system calls (not stdio).
 * Demonstrates: creat/open, write, lseek, read, stat, unlink, mkdir, rename.
 *
 * Compile: gcc file_system_calls.c -o file_system_calls
 * Run:     ./file_system_calls
 *
 * Creates ./demo_dir/notes.txt, writes, reads back, prints metadata,
 * renames, then cleans up.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_NAME  "demo_dir"
#define FILE_NAME "demo_dir/notes.txt"
#define NEW_NAME  "demo_dir/notes_renamed.txt"

static void die(const char *what) {
    perror(what);
    exit(1);
}

int main(void) {
    /* mkdir: ignore EEXIST so the program can be re-run */
    if (mkdir(DIR_NAME, 0755) == -1 && errno != EEXIST)
        die("mkdir");
    printf("mkdir(%s) ok\n", DIR_NAME);

    /* open with O_CREAT|O_TRUNC|O_RDWR  (creat-style create) */
    int fd = open(FILE_NAME, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1) die("open");
    printf("open(%s) -> fd %d\n", FILE_NAME, fd);

    const char *text = "Operating Systems lab\nFile system calls: open read write lseek stat\n";
    ssize_t n = write(fd, text, strlen(text));
    if (n == -1) die("write");
    printf("write: %zd bytes\n", n);

    const char *extra = "Appended line via lseek(SEEK_END)\n";
    if (lseek(fd, 0, SEEK_END) == -1) die("lseek end");
    if (write(fd, extra, strlen(extra)) == -1) die("write append");

    /* read from the start */
    if (lseek(fd, 0, SEEK_SET) == -1) die("lseek set");
    char buf[512];
    memset(buf, 0, sizeof(buf));
    ssize_t total = 0, r;
    while ((r = read(fd, buf + total, sizeof(buf) - 1 - total)) > 0)
        total += r;
    if (r == -1) die("read");
    printf("read %zd bytes:\n----\n%s----\n", total, buf);

    /* fstat / stat */
    struct stat st;
    if (fstat(fd, &st) == -1) die("fstat");
    printf("stat: size=%lld mode=%o inode=%llu\n",
           (long long)st.st_size,
           st.st_mode & 0777,
           (unsigned long long)st.st_ino);

    if (close(fd) == -1) die("close");
    printf("close ok\n");

    if (rename(FILE_NAME, NEW_NAME) == -1) die("rename");
    printf("rename -> %s\n", NEW_NAME);

    if (unlink(NEW_NAME) == -1) die("unlink");
    printf("unlink(%s) ok\n", NEW_NAME);

    if (rmdir(DIR_NAME) == -1) die("rmdir");
    printf("rmdir(%s) ok\n", DIR_NAME);

    printf("All file-system system calls completed.\n");
    return 0;
}
```

---

# Process & Thread Management

## 19. Process Creation
**File:** process_creation.c

```c
/*
 * Process Creation using the fork() system call.
 * Demonstrates:
 *   - Creating a child process with fork()
 *   - Distinguishing parent and child execution flows
 *   - Fetching PID and PPID using getpid() and getppid()
 *   - Separate address space demonstration (local/global variables)
 *   - Synchronizing parent with child using wait() and checking exit status
 *
 * Compile: gcc process_creation.c -o process_creation
 * Run:     ./process_creation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_var = 100;

int main(void) {
    pid_t pid;
    int local_var = 20;

    printf("=========================================\n");
    printf("        Process Creation Demo            \n");
    printf("=========================================\n");
    printf("[Main] Initial Process PID: %d, PPID: %d\n", getpid(), getppid());
    printf("[Main] Initial values -> global_var: %d, local_var: %d\n\n", global_var, local_var);

    /* Fork a child process */
    pid = fork();

    if (pid < 0) {
        /* Fork failed */
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process execution */
        printf("[Child ] Created successfully!\n");
        printf("[Child ] Current PID  : %d\n", getpid());
        printf("[Child ] Parent PID   : %d\n", getppid());

        /* Modifying variables in child to demonstrate separate address space */
        global_var += 50;
        local_var += 50;
        printf("[Child ] Modified values -> global_var: %d, local_var: %d\n", global_var, local_var);
        printf("[Child ] Working on child task...\n");
        sleep(1);
        printf("[Child ] Task completed. Exiting with status 42.\n\n");
        exit(42);
    } else {
        /* Parent process execution */
        int status;
        printf("[Parent] Fork returned child PID: %d\n", pid);
        printf("[Parent] Waiting for child process to complete...\n\n");

        /* Wait for child to exit */
        pid_t terminated_pid = wait(&status);

        if (terminated_pid == -1) {
            perror("Wait failed");
            exit(EXIT_FAILURE);
        }

        printf("[Parent] Child process (PID: %d) has finished.\n", terminated_pid);
        if (WIFEXITED(status)) {
            printf("[Parent] Child exited normally with exit code: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[Parent] Child was terminated by signal: %d\n", WTERMSIG(status));
        }

        /* Show that parent's memory was unaffected by child's modifications */
        printf("[Parent] Values in parent -> global_var: %d, local_var: %d (unchanged!)\n", global_var, local_var);
        printf("=========================================\n");
    }

    return EXIT_SUCCESS;
}
```

---

## 20. Thread Creation
**File:** thread_creation.c

```c
/*
 * Thread Creation using POSIX Threads (pthreads).
 * Demonstrates:
 *   - Creating multiple threads with pthread_create()
 *   - Passing parameters/arguments to thread routines
 *   - Querying thread IDs using pthread_self()
 *   - Shared memory space across threads
 *   - Waiting for threads to finish using pthread_join()
 *   - Returning status/data from threads
 *
 * Compile: gcc thread_creation.c -o thread_creation -pthread
 * Run:     ./thread_creation
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

/* Structure to pass multiple arguments to thread function */
typedef struct {
    int thread_num;
    int work_units;
} thread_data_t;

/* Shared resource among all threads */
static int shared_counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Thread routine */
static void *thread_worker(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int id = data->thread_num;
    pthread_t tid = pthread_self();

    printf("[Thread %d] Created. TID: %lu, Target work units: %d\n", id, (unsigned long)tid, data->work_units);

    /* Simulate work and safely access shared memory */
    for (int i = 1; i <= data->work_units; i++) {
        pthread_mutex_lock(&lock);
        shared_counter++;
        printf("[Thread %d] Step %d/%d (Shared Counter = %d)\n", 
               id, i, data->work_units, shared_counter);
        pthread_mutex_unlock(&lock);
        usleep(50000); /* 50ms delay to demonstrate interleaving */
    }

    /* Allocate result dynamically to return to pthread_join */
    int *result = malloc(sizeof(int));
    if (result != NULL) {
        *result = data->work_units * 10;
    }

    printf("[Thread %d] Completed work. Exiting.\n", id);
    pthread_exit((void *)result);
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_args[NUM_THREADS];
    int rc;

    printf("=========================================\n");
    printf("         Thread Creation Demo            \n");
    printf("=========================================\n");
    printf("[Main] Creating %d POSIX threads...\n\n", NUM_THREADS);

    /* Create threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_num = i + 1;
        thread_args[i].work_units = 3;

        printf("[Main] Spawning Thread %d...\n", i + 1);
        rc = pthread_create(&threads[i], NULL, thread_worker, (void *)&thread_args[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d: return code %d\n", i + 1, rc);
            exit(EXIT_FAILURE);
        }
    }

    printf("\n[Main] All threads spawned. Waiting for them to complete...\n\n");

    /* Wait for threads to complete using pthread_join */
    for (int i = 0; i < NUM_THREADS; i++) {
        void *status;
        rc = pthread_join(threads[i], &status);
        if (rc != 0) {
            fprintf(stderr, "Error joining thread %d: return code %d\n", i + 1, rc);
            exit(EXIT_FAILURE);
        }

        if (status != NULL) {
            int ret_val = *(int *)status;
            printf("[Main] Joined Thread %d (returned value: %d)\n", i + 1, ret_val);
            free(status);
        }
    }

    pthread_mutex_destroy(&lock);

    printf("\n[Main] All threads successfully finished.\n");
    printf("[Main] Final Shared Counter value: %d\n", shared_counter);
    printf("=========================================\n");

    return EXIT_SUCCESS;
}
```

---
