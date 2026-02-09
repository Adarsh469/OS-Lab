#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id, at, bt, pri, wt, tat, ct, rem_bt;
} Process;

void reset_data(Process p[], int n) {
    for (int i = 0; i < n; i++) p[i].rem_bt = p[i].bt;
}

void display(Process p[], int n) {
    float avg_wt = 0, avg_tat = 0;
    printf("\nID\tAT\tBT\tPri\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].pri, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage WT: %.2f | Average TAT: %.2f\n", avg_wt / n, avg_tat / n);
}

void fcfs(Process p[], int n) {
    int cur = 0;
    printf("\nGantt Chart: ");
    for (int i = 0; i < n; i++) {
        if (cur < p[i].at) cur = p[i].at;
        cur += p[i].bt;
        p[i].ct = cur;
        printf("| P%d (%d) ", p[i].id, cur);
    }
    printf("|\n");
    display(p, n);
}

void priority_np(Process p[], int n) {
    int cur = 0, completed = 0;
    int visited[10] = {0};
    printf("\nGantt Chart: ");
    while (completed < n) {
        int idx = -1, max_pri = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= cur && !visited[i]) {
                if (p[i].pri > max_pri) { // Higher number = Higher Priority
                    max_pri = p[i].pri;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            cur += p[idx].bt;
            p[idx].ct = cur;
            visited[idx] = 1;
            completed++;
            printf("| P%d (%d) ", p[idx].id, cur);
        } else cur++;
    }
    printf("|\n");
    display(p, n);
}

void round_robin(Process p[], int n, int q) {
    int cur = 0, completed = 0;
    reset_data(p, n);
    printf("\nGantt Chart: ");
    while (completed < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= cur && p[i].rem_bt > 0) {
                found = 1;
                int take = (p[i].rem_bt > q) ? q : p[i].rem_bt;
                p[i].rem_bt -= take;
                cur += take;
                printf("| P%d (%d) ", p[i].id, cur);
                if (p[i].rem_bt == 0) {
                    p[i].ct = cur;
                    completed++;
                }
            }
        }
        if (!found) cur++;
    }
    printf("|\n");
    display(p, n);
}

void srtf(Process p[], int n) {
    int cur = 0, completed = 0, prev = -1;
    reset_data(p, n);
    printf("\nGantt Chart: ");
    while (completed < n) {
        int idx = -1, min_bt = 10000;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= cur && p[i].rem_bt > 0 && p[i].rem_bt < min_bt) {
                min_bt = p[i].rem_bt;
                idx = i;
            }
        }
        if (idx != -1) {
            if (prev != idx) printf("| P%d ", p[idx].id);
            p[idx].rem_bt--;
            cur++;
            if (p[idx].rem_bt == 0) {
                p[idx].ct = cur;
                completed++;
                printf("(%d) ", cur);
            }
            prev = idx;
        } else cur++;
    }
    printf("|\n");
    display(p, n);
}

int main() {
    int n, choice, q = 10;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("P%d [AT BT Priority]: ", i + 1);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pri);
    }

    while (1) {
        printf("\n1. FCFS\n2. SRTF\n3. Round Robin (q=10)\n4. Priority (NP)\n5. Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fcfs(p, n); break;
            case 2: srtf(p, n); break;
            case 3: round_robin(p, n, q); break;
            case 4: priority_np(p, n); break;
            case 5: exit(0);
        }
    }
    return 0;
}