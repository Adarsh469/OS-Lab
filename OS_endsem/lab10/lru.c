/**
 * LRU Page Replacement
 * Compile: gcc lru.c -o lru
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n, frames_count;
    
    printf("\n========== LRU PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *time = (int*)malloc(frames_count * sizeof(int));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) { frames[i] = -1; time[i] = 0; }
    
    int faults = 0, counter = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) { found = true; pos = j; break; }
        }
        printf("%d\t", pages[i]);
        if (found) {
            time[pos] = counter++;
        } else {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = pages[i];
                time[empty] = counter++;
            } else {
                int lru = 0;
                for (int j = 1; j < frames_count; j++) if (time[j] < time[lru]) lru = j;
                frames[lru] = pages[i];
                time[lru] = counter++;
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames); free(time);
    return 0;
}