/**
 * LFU Page Replacement
 * Replace page with smallest frequency count. Tie: FIFO among ties.
 * Compile: gcc lfu.c -o lfu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n, frames_count;
    
    printf("\n========== LFU PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *freq = (int*)malloc(frames_count * sizeof(int));
    int *load_time = (int*)malloc(frames_count * sizeof(int));  // for tie-breaking
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) { frames[i] = -1; freq[i] = 0; load_time[i] = 0; }
    
    int faults = 0, timer = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) { found = true; pos = j; break; }
        }
        printf("%d\t", pages[i]);
        if (found) {
            freq[pos]++;
        } else {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = pages[i];
                freq[empty] = 1;
                load_time[empty] = timer++;
            } else {
                int lfu = 0;
                for (int j = 1; j < frames_count; j++) {
                    if (freq[j] < freq[lfu] || (freq[j] == freq[lfu] && load_time[j] < load_time[lfu]))
                        lfu = j;
                }
                frames[lfu] = pages[i];
                freq[lfu] = 1;
                load_time[lfu] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames); free(freq); free(load_time);
    return 0;
}