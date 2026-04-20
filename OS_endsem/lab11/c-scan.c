/**
 * C-SCAN Disk Scheduling
 * Compile: gcc cscan.c -o cscan
 */

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main() {
    int n, head, disk_size;
    
    printf("\n========== C-SCAN DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc((n + 3) * sizeof(int));
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size (max cylinder, e.g., 199): ");
    scanf("%d", &disk_size);
    
    requests[n] = head;
    requests[n + 1] = 0;
    requests[n + 2] = disk_size;
    qsort(requests, n + 3, sizeof(int), compare);
    
    int pos = 0;
    while (requests[pos] != head) pos++;
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    // Move right to end
    for (int i = pos + 1; i < n + 3; i++) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    // Jump to beginning (seek is counted in manual? Usually yes)
    total_seek += abs(disk_size - 0);  // jump from end to 0
    current = 0;
    printf(" -> 0");
    // Continue from start to left of head
    for (int i = 0; i < pos; i++) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}