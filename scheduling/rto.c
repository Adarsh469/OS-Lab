#include <stdio.h>
#include <pthread.h>

#define QUANTUM 4

int n;
int pid[10], at[10], bt[10], priority[10];
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
            if (at[a] > at[b] || (at[a] == at[b] && priority[a] > priority[b])) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }
}

void *thread1(void *arg) {
    sort_by_arrival();

    printf("\nThread 1: Ready Queue Order:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d", pid[order[i]]);
        if (i < n - 1) printf(" -> ");
    }
    printf("\n");
    return NULL;
}

void *thread2(void *arg) {
    int time = 0, completed = 0;
    int in_queue[10] = {0};
    int queue[200], front = 0, rear = 0;
    int gantt_pid[200], gantt_time[201], g = 0;
    int overrun[10] = {0};

    for (int i = 0; i < n; i++)
        remaining[i] = bt[i];

    queue[rear++] = order[0];
    in_queue[order[0]] = 1;

    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && remaining[i] > 0 && at[i] <= time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int p = queue[front++];

        int run = (remaining[p] < QUANTUM) ? remaining[p] : QUANTUM;

        if (remaining[p] > QUANTUM && remaining[p] < QUANTUM + 2) {
            run = remaining[p];
            overrun[p] = remaining[p] - QUANTUM;
        }

        gantt_time[g] = time;
        gantt_pid[g] = pid[p];
        g++;

        time += run;
        remaining[p] -= run;

        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && remaining[i] > 0 && at[i] <= time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (remaining[p] == 0) {
            ct[p] = time;
            tat[p] = ct[p] - at[p];
            wt[p] = tat[p] - bt[p];
            completed++;

            if (overrun[p] > 0)
                printf("P%d had an overrun of %d unit(s) beyond quantum\n", pid[p], overrun[p]);
        } else {
            queue[rear++] = p;
        }
    }
    gantt_time[g] = time;

    printf("\nThread 2: RTO Scheduling (Time Quantum = %d, Max Overrun = 1)\n", QUANTUM);
    printf("Gantt Chart\n");
    for (int i = 0; i <= g; i++) printf("%-5d", gantt_time[i]);
    printf("\n|");
    for (int i = 0; i < g; i++) printf(" P%-2d|", gantt_pid[i]);
    printf("\n");

    double total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += tat[i];
        total_wt += wt[i];
    }

    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time = %.2f\n", total_wt / n);

    printf("\n--- Final Scheduling Table ---\n");
    printf("%-6s %-5s %-5s %-10s %-5s %-5s %-5s\n", "PID", "AT", "BT", "Priority", "CT", "TAT", "WT");
    for (int i = 0; i < n; i++)
        printf("P%-5d %-5d %-5d %-10d %-5d %-5d %-5d\n", pid[i], at[i], bt[i], priority[i], ct[i], tat[i], wt[i]);

    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter PID, Arrival Time, Burst Time, Priority for each process:\n");
    for (int i = 0; i < n; i++)
        scanf("%d %d %d %d", &pid[i], &at[i], &bt[i], &priority[i]);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t2, NULL);

    return 0;
}