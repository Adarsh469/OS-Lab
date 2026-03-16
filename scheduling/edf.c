#include <stdio.h>
#include <pthread.h>

int n;
int pid[10], at[10], bt[10], deadline[10];
int ct[10], tat[10], wt[10];
int order[10];
int remaining[10];

void sort_by_arrival() {
    int i, j, temp;
    for (i = 0; i < n; i++)
        order[i] = i;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            int a = order[i], b = order[j];
            if (at[a] > at[b] || (at[a] == at[b] && deadline[a] > deadline[b])) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }
}

void *thread1(void *arg) {
    sort_by_arrival();

    printf("\nThread 1: Ready Queue Order (by Arrival, then Deadline):\n");
    for (int i = 0; i < n; i++) {
        printf("P%d", pid[order[i]]);
        if (i < n - 1) printf(" -> ");
    }
    printf("\n");
    return NULL;
}

void *thread2(void *arg) {
    int time = 0, completed = 0;
    int done[10] = {0};
    int gantt_pid[500], gantt_time[501], g = 0;
    int prev = -1;

    for (int i = 0; i < n; i++)
        remaining[i] = bt[i];

    while (completed < n) {
        int earliest = -1;
        int min_deadline = 99999;

        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= time && deadline[i] < min_deadline) {
                min_deadline = deadline[i];
                earliest = i;
            }
        }

        if (earliest == -1) {
            time++;
            continue;
        }

        if (earliest != prev) {
            gantt_time[g] = time;
            gantt_pid[g] = pid[earliest];
            g++;
            prev = earliest;
        }

        remaining[earliest]--;
        time++;

        if (remaining[earliest] == 0) {
            ct[earliest] = time;
            tat[earliest] = ct[earliest] - at[earliest];
            wt[earliest] = tat[earliest] - bt[earliest];
            done[earliest] = 1;
            completed++;
            prev = -1;
        }
    }
    gantt_time[g] = time;

    printf("\nThread 2: EDF (Earliest Deadline First) Scheduling\n");
    printf("Gantt Chart\n");
    for (int i = 0; i <= g; i++) printf("%-5d", gantt_time[i]);
    printf("\n|");
    for (int i = 0; i < g; i++) printf(" P%-2d|", gantt_pid[i]);
    printf("\n");

    printf("\nDeadline Miss Report:\n");
    for (int i = 0; i < n; i++) {
        if (ct[i] > deadline[i])
            printf("P%d MISSED deadline  (finished at %d, deadline was %d)\n", pid[i], ct[i], deadline[i]);
        else
            printf("P%d met deadline     (finished at %d, deadline was %d)\n", pid[i], ct[i], deadline[i]);
    }

    double total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += tat[i];
        total_wt += wt[i];
    }

    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time = %.2f\n", total_wt / n);

    printf("\n--- Final Scheduling Table ---\n");
    printf("%-6s %-5s %-5s %-10s %-5s %-5s %-5s\n", "PID", "AT", "BT", "Deadline", "CT", "TAT", "WT");
    for (int i = 0; i < n; i++)
        printf("P%-5d %-5d %-5d %-10d %-5d %-5d %-5d\n", pid[i], at[i], bt[i], deadline[i], ct[i], tat[i], wt[i]);

    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter PID, Arrival Time, Burst Time, Deadline for each process:\n");
    for (int i = 0; i < n; i++)
        scanf("%d %d %d %d", &pid[i], &at[i], &bt[i], &deadline[i]);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t2, NULL);

    return 0;
}
