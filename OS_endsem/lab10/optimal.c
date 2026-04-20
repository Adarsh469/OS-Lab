/**
 * Optimal Page Replacement
 * Compile: gcc optimal.c -o optimal
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int main() {
    int n, frames_count;
    
    printf("\n========== OPTIMAL PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) frames[i] = -1;
    
    int faults = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) { found = true; break; }
        }
        printf("%d\t", pages[i]);
        if (!found) {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = pages[i];
            } else {
                int replace = -1, farthest = -1;
                for (int j = 0; j < frames_count; j++) {
                    int next = INT_MAX;
                    for (int k = i+1; k < n; k++) if (pages[k] == frames[j]) { next = k; break; }
                    if (next > farthest) { farthest = next; replace = j; }
                }
                frames[replace] = pages[i];
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames);
    return 0;
}