/**
 * LOOK Disk Scheduling
 * Compile: gcc look.c -o look
 */

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main() {
    int n, head, direction;
    
    printf("\n========== LOOK DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc((n + 1) * sizeof(int));
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter direction (0=left, 1=right): ");
    scanf("%d", &direction);
    
    requests[n] = head;
    qsort(requests, n + 1, sizeof(int), compare);
    
    int pos = 0;
    while (requests[pos] != head) pos++;
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    if (direction == 1) {
        for (int i = pos + 1; i <= n; i++) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
        for (int i = pos - 1; i >= 0; i--) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
    } else {
        for (int i = pos - 1; i >= 0; i--) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
        for (int i = pos + 1; i <= n; i++) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}