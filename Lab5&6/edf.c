#include <stdio.h>

int main() {
    int n, time = 0, total_bt = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    int bt[n], deadline[n], period[n], rem_bt[n];

    for (int i = 0; i < n; i++) {
        printf("P%d [Burst, Deadline, Period]: ", i + 1);
        scanf("%d %d %d", &bt[i], &deadline[i], &period[i]);
        rem_bt[i] = bt[i];
        total_bt += bt[i];
    }

    printf("\nGantt Chart:\n0");
    while (time < 200) { 
        int selected = -1;
        int min_deadline = 10000;

        for (int i = 0; i < n; i++) {
            if (time % period[i] == 0) rem_bt[i] = bt[i]; // New period starts
            
            if (rem_bt[i] > 0) {
                // Calculate current deadline relative to time
                int current_deadline = (time / period[i] + 1) * period[i];
                if (current_deadline < min_deadline) {
                    min_deadline = current_deadline;
                    selected = i;
                }
            }
        }

        if (selected != -1) {
            rem_bt[selected]--;
            time++;
            printf("--P%d--%d", selected + 1, time);
        } else {
            time++;
            printf("--IDLE--%d", time);
        }
    }
    return 0;
}