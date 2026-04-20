/**
 * Second Chance (Clock) Page Replacement
 * Compile: gcc second_chance.c -o second_chance
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n, frames_count;
    
    printf("\n========== SECOND CHANCE (CLOCK) PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *ref_bit = (int*)malloc(frames_count * sizeof(int));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) { frames[i] = -1; ref_bit[i] = 0; }
    
    int faults = 0, pointer = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) {
                found = true;
                ref_bit[j] = 1;
                break;
            }
        }
        printf("%d\t", pages[i]);
        if (!found) {
            while (1) {
                if (frames[pointer] == -1) {
                    frames[pointer] = pages[i];
                    ref_bit[pointer] = 1;
                    pointer = (pointer + 1) % frames_count;
                    break;
                } else if (ref_bit[pointer] == 0) {
                    frames[pointer] = pages[i];
                    ref_bit[pointer] = 1;
                    pointer = (pointer + 1) % frames_count;
                    break;
                } else {
                    ref_bit[pointer] = 0;
                    pointer = (pointer + 1) % frames_count;
                }
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames); free(ref_bit);
    return 0;
}