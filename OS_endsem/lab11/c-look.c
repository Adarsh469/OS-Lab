/**
 * C-LOOK Disk Scheduling
 * Compile: gcc clook.c -o clook
 */

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main() {
    int n, head;
    
    printf("\n========== C-LOOK DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc((n + 1) * sizeof(int));
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    requests[n] = head;
    qsort(requests, n + 1, sizeof(int), compare);
    
    int pos = 0;
    while (requests[pos] != head) pos++;
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    // Move right to highest request
    for (int i = pos + 1; i <= n; i++) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    // Jump to lowest request (seek counted)
    total_seek += abs(current - requests[0]);
    current = requests[0];
    printf(" -> %d", current);
    // Continue to left of head
    for (int i = 1; i < pos; i++) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}