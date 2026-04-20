/**
 * Question 1: LSTF Disk Scheduling + MFU Page Replacement
 * Compile: gcc lstf_mfu.c -o lstf_mfu
 * Run: ./lstf_mfu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_INPUT 100

typedef struct {
    int page;
    int cylinder;
    bool serviced;
} Request;

// LSTF Disk Scheduling: returns page reference string in the order of servicing
void lstf_schedule(Request requests[], int n, int head, int *page_seq) {
    int current = head;
    for (int count = 0; count < n; count++) {
        int max_dist = -1;
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!requests[i].serviced) {
                int dist = abs(requests[i].cylinder - current);
                if (dist > max_dist) {
                    max_dist = dist;
                    idx = i;
                }
            }
        }
        requests[idx].serviced = true;
        current = requests[idx].cylinder;
        page_seq[count] = requests[idx].page;
    }
}

// MFU Page Replacement
void mfu_page_replacement(int pages[], int n, int frames_count) {
    int *frames = (int*)malloc(frames_count * sizeof(int));
    int *freq = (int*)malloc(frames_count * sizeof(int));
    int *load_time = (int*)malloc(frames_count * sizeof(int));
    
    for (int i = 0; i < frames_count; i++) {
        frames[i] = -1;
        freq[i] = 0;
        load_time[i] = 0;
    }
    
    int faults = 0, timer = 0;
    printf("\n--- MFU Page Replacement ---\n");
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
                // Find MFU (highest frequency, tie by oldest load_time)
                int mfu = 0;
                for (int j = 1; j < frames_count; j++) {
                    if (freq[j] > freq[mfu] || (freq[j] == freq[mfu] && load_time[j] < load_time[mfu]))
                        mfu = j;
                }
                frames[mfu] = page;
                freq[mfu] = 1;
                load_time[mfu] = timer++;
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
    
    printf("\n========== LSTF + MFU COMBINATION ==========\n");
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
        requests[n].serviced = false;
        n++;
    }
    
    // Apply LSTF to get page sequence
    int *page_seq = (int*)malloc(n * sizeof(int));
    lstf_schedule(requests, n, head, page_seq);
    
    printf("\nLSTF Disk Scheduling Order (cylinder sequence):\n");
    int curr = head;
    printf("Head %d", curr);
    for (int i = 0; i < n; i++) {
        printf(" -> %d", requests[i].cylinder); // order is already by service
        curr = requests[i].cylinder;
    }
    printf("\nGenerated Page Reference String: ");
    for (int i = 0; i < n; i++) printf("%d ", page_seq[i]);
    printf("\n");
    
    // Apply MFU
    mfu_page_replacement(page_seq, n, frames_count);
    
    free(page_seq);
    return 0;
}