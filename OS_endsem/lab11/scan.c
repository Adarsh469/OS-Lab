/**
 * SCAN Disk Scheduling
 * Compile: gcc scan.c -o scan
 */

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main() {
    int n, head, disk_size, direction;
    
    printf("\n========== SCAN (ELEVATOR) DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc((n + 3) * sizeof(int));
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size (max cylinder, e.g., 199): ");
    scanf("%d", &disk_size);
    printf("Enter direction (0=left/towards 0, 1=right/towards end): ");
    scanf("%d", &direction);
    
    // Add head to array for sorting, but we'll handle it separately
    requests[n] = head;
    requests[n + 1] = 0;          // end points
    requests[n + 2] = disk_size;
    qsort(requests, n + 3, sizeof(int), compare);
    
    // Find head position in sorted array
    int pos = 0;
    while (requests[pos] != head) pos++;
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    if (direction == 1) {  // moving right
        for (int i = pos + 1; i < n + 3; i++) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
        for (int i = pos - 1; i >= 0; i--) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
    } else {  // moving left
        for (int i = pos - 1; i >= 0; i--) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
        for (int i = pos + 1; i < n + 3; i++) {
            total_seek += abs(requests[i] - current);
            current = requests[i];
            printf(" -> %d", current);
        }
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}