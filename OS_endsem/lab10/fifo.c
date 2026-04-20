/**
 * FIFO Page Replacement
 * Compile: gcc fifo.c -o fifo
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n, frames_count;
    
    printf("\n========== FIFO PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) frames[i] = -1;
    
    int faults = 0, pointer = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) { found = true; break; }
        }
        printf("%d\t", pages[i]);
        if (!found) {
            frames[pointer] = pages[i];
            pointer = (pointer + 1) % frames_count;
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