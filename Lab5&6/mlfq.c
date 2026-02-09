#include <stdio.h>

struct proc {
    int id, at, bt, rt;
} q0[10], q1[10], q2[10];

int main() {
    int n, i, time = 0, f0 = 0, r0 = 0, f1 = 0, r1 = 0, f2 = 0, r2 = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        q0[r0].id = i + 1;
        printf("Burst time for P%d: ", i + 1);
        scanf("%d", &q0[r0].bt);
        q0[r0].rt = q0[r0].bt;
        r0++;
    }

    printf("\nGantt Chart: ");
    // Queue 0 (RR quantum 8)
    while (f0 < r0) {
        if (q0[f0].rt <= 8) {
            time += q0[f0].rt;
            printf("| P%d(%d) ", q0[f0].id, time);
            f0++;
        } else {
            time += 8;
            q0[f0].rt -= 8;
            printf("| P%d(%d) ", q0[f0].id, time);
            q1[r1++] = q0[f0++];
        }
    }

    // Queue 1 (RR quantum 16)
    while (f1 < r1) {
        if (q1[f1].rt <= 16) {
            time += q1[f1].rt;
            printf("| P%d(%d) ", q1[f1].id, time);
            f1++;
        } else {
            time += 16;
            q1[f1].rt -= 16;
            printf("| P%d(%d) ", q1[f1].id, time);
            q2[r2++] = q1[f1++];
        }
    }

    // Queue 2 (FCFS)
    while (f2 < r2) {
        time += q2[f2].rt;
        printf("| P%d(%d) ", q2[f2].id, time);
        f2++;
    }
    printf("|\n");
    return 0;
}