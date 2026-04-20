/**
 * LSTF Disk Scheduling (Largest Seek Time First)
 * Opposite of SSTF: always pick farthest request
 * Compile: gcc lstf.c -o lstf
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int main() {
    int n, head;
    
    printf("\n========== LSTF DISK SCHEDULING ==========\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int*)malloc(n * sizeof(int));
    bool *serviced = (bool*)calloc(n, sizeof(bool));
    
    printf("Enter request queue (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("\nSeek Sequence: %d", head);
    int total_seek = 0, current = head;
    
    for (int count = 0; count < n; count++) {
        int max_dist = -1, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!serviced[i]) {
                int dist = abs(requests[i] - current);
                if (dist > max_dist) {
                    max_dist = dist;
                    idx = i;
                }
            }
        }
        serviced[idx] = true;
        total_seek += max_dist;
        current = requests[idx];
        printf(" -> %d", current);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total_seek);
    
    free(requests); free(serviced);
    return 0;
}