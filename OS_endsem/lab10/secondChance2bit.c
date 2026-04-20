/**
 * Enhanced Second Chance (R,M bits) Page Replacement
 * Priority: (0,0) > (0,1) > (1,0) > (1,1)
 * Modified bit is randomly simulated (since we don't have write info)
 * Compile: gcc enhanced_second.c -o enhanced_second
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int main() {
    int n, frames_count;
    srand(time(0));
    
    printf("\n========== ENHANCED SECOND CHANCE PAGE REPLACEMENT ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *ref_bit = (int*)malloc(frames_count * sizeof(int));
    int *mod_bit = (int*)malloc(frames_count * sizeof(int));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) { frames[i] = -1; ref_bit[i] = 0; mod_bit[i] = 0; }
    
    int faults = 0;
    printf("\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        bool is_write = rand() % 2;  // simulate read/write
        bool found = false;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) {
                found = true;
                ref_bit[j] = 1;
                if (is_write) mod_bit[j] = 1;
                break;
            }
        }
        printf("%d\t", pages[i]);
        if (!found) {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = pages[i];
                ref_bit[empty] = 1;
                mod_bit[empty] = is_write;
            } else {
                // Find best class: 0=(0,0), 1=(0,1), 2=(1,0), 3=(1,1)
                int best_class = 4, replace = -1;
                for (int pass = 0; pass < 4 && replace == -1; pass++) {
                    for (int j = 0; j < frames_count; j++) {
                        int cls = (ref_bit[j] << 1) | mod_bit[j];
                        if (cls == pass) { replace = j; break; }
                    }
                    if (pass == 3 && replace == -1) {  // all (1,1), clear ref bits and restart
                        for (int j = 0; j < frames_count; j++) ref_bit[j] = 0;
                        pass = -1;
                    }
                }
                frames[replace] = pages[i];
                ref_bit[replace] = 1;
                mod_bit[replace] = is_write;
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames); free(ref_bit); free(mod_bit);
    return 0;
}