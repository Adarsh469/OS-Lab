/**
 * Question 2: LCFS Disk Scheduling + LFU Page Replacement
 * Compile: gcc lcfs_lfu.c -o lcfs_lfu
 * Run: ./lcfs_lfu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_INPUT 100

typedef struct {
    int page;
    int cylinder;
} Request;

// LCFS Disk Scheduling: service in reverse order of arrival (stack)
void lcfs_schedule(Request requests[], int n, int head, int *page_seq) {
    // The requests array is in arrival order; LCFS services from last to first
    int current = head;
    for (int i = n - 1; i >= 0; i--) {
        page_seq[n - 1 - i] = requests[i].page; // store in service order
        // For display we could track movement but we just need page sequence
    }
}

// LFU Page Replacement
void lfu_page_replacement(int pages[], int n, int frames_count) {
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *freq = (int*)malloc(frames_count * sizeof(int));
    int *load_time = (int*)malloc(frames_count * sizeof(int));
    
    for (int i = 0; i < frames_count; i++) {
        frames[i] = -1;
        freq[i] = 0;
        load_time[i] = 0;
    }
    
    int faults = 0, timer = 0;
    printf("\n--- LFU Page Replacement ---\n");
    printf("Reference String: ");
    for (int i = 0; i < n; i++) printf("%d ", pages[i]);
    printf("\n\nRef\tFrames\t\tFault?\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        bool found = false;
        int pos = -1;
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == page) {
                found = true;
                pos = j;
                break;
            }
        }
        printf("%d\t", page);
        if (found) {
            freq[pos]++;
        } else {
            int empty = -1;
            for (int j = 0; j < frames_count; j++) if (frames[j] == -1) { empty = j; break; }
            if (empty != -1) {
                frames[empty] = page;
                freq[empty] = 1;
                load_time[empty] = timer++;
            } else {
                // Find LFU (lowest frequency, tie by oldest load_time)
                int lfu = 0;
                for (int j = 1; j < frames_count; j++) {
                    if (freq[j] < freq[lfu] || (freq[j] == freq[lfu] && load_time[j] < load_time[lfu]))
                        lfu = j;
                }
                frames[lfu] = page;
                freq[lfu] = 1;
                load_time[lfu] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames_count; j++)
            printf("%s%d", j ? " " : "", frames[j] == -1 ? -1 : frames[j]);
        printf("\t\t%s\n", found ? "Hit" : "Fault");
    }
    printf("\nTotal Page Faults: %d, Hit Ratio: %.2f%%\n", faults, ((float)(n - faults) / n) * 100);
    
    free(frames); free(freq); free(load_time);
}

int main() {
    int frames_count, head;
    Request requests[MAX_INPUT];
    int n = 0;
    
    printf("\n========== LCFS + LFU COMBINATION ==========\n");
    printf("Enter number of frames: ");
    scanf("%d", &frames_count);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter pairs (page cylinder) and -1 to end:\n");
    
    while (1) {
        int page, cyl;
        scanf("%d", &page);
        if (page == -1) break;
        scanf("%d", &cyl);
        requests[n].page = page;
        requests[n].cylinder = cyl;
        n++;
    }
    
    // Apply LCFS to get page sequence
    int *page_seq = (int*)malloc(n * sizeof(int));
    lcfs_schedule(requests, n, head, page_seq);
    
    printf("\nLCFS Disk Scheduling Order (cylinder sequence):\n");
    printf("Head %d", head);
    int curr = head, total = 0;
    for (int i = n - 1; i >= 0; i--) {
        total += abs(requests[i].cylinder - curr);
        curr = requests[i].cylinder;
        printf(" -> %d", curr);
    }
    printf("\nTotal Head Movement: %d cylinders\n", total);
    printf("Generated Page Reference String: ");
    for (int i = 0; i < n; i++) printf("%d ", page_seq[i]);
    printf("\n");
    
    // Apply LFU
    lfu_page_replacement(page_seq, n, frames_count);
    
    free(page_seq);
    return 0;
}