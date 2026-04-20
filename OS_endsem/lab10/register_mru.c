/**
 * MRU using 8-bit Reference Registers (Exam variation)
 * Shift right, set MSB=1 on reference, replace LARGEST value.
 * Compile: gcc ref_register_mru.c -o ref_register_mru
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define REG_SIZE 8

int main() {
    int n, frames_count;
    
    printf("\n========== MRU WITH 8-BIT REFERENCE REGISTERS ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter length of reference string: ");
    scanf("%d", &n);
    
    int *pages = (int*)malloc(n * sizeof(int));
    int *frames = (int*)malloc(frames_count * sizeof(int));
    unsigned char *reg = (unsigned char*)malloc(frames_count * sizeof(unsigned char));
    
    printf("Enter reference string (space-separated):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frames_count; i++) { frames[i] = -1; reg[i] = 0; }
    
    int faults = 0;
    printf("\nRef\tFrames\t\tRegisters\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < frames_count; j++) reg[j] >>= 1;
        
        bool found = false;
        int pos = -1;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == pages[i]) { found = true; pos = j; break; }
        }
        printf("%d\t", pages[i]);
        if (found) {
            reg[pos] |= 0x80;
        } else {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = pages[i];
                reg[empty] = 0x80;
            } else {
                int mru = 0;
                for (int j = 1; j < frames_count; j++) if (reg[j] > reg[mru]) mru = j;  // LARGEST
                frames[mru] = pages[i];
                reg[mru] = 0x80;
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]); else printf("- ");
        }
        printf("\t");
        for (int j = 0; j < frames_count; j++) {
            for (int b = 7; b >= 0; b--) printf("%d", (reg[j] >> b) & 1);
            printf(" ");
        }
        printf("\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    
    free(pages); free(frames); free(reg);
    return 0;
}