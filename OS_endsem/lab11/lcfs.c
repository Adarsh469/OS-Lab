/**
 * LCFS Disk Scheduling (Last Come First Serve)
 * Process requests in reverse order of arrival (stack)
 * Compile: gcc lcfs.c -o lcfs
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head;
    
    printf("\n========== LCFS DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc(n * sizeof(int));
    printf("Enter request queue in order of arrival (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    // Process from last to first (LIFO)
    for (int i = n - 1; i >= 0; i--) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}