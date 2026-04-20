/**
 * FCFS Disk Scheduling
 * Compile: gcc fcfs.c -o fcfs
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head;
    
    printf("\n========== FCFS DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc(n * sizeof(int));
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    for (int i = 0; i < n; i++) {
        total_seek += abs(requests[i] - current);
        current = requests[i];
        printf(" -> %d", current);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests);
    return 0;
}