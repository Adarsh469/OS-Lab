/**
 * DISK SCHEDULING + PAGE REPLACEMENT - PLUG AND PLAY FRAMEWORK
 * Compile: gcc disk_page_combo.c -o disk_page_combo
 * Run: ./disk_page_combo
 */

// ========== 1. HEADERS ==========
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define MAX_INPUT 100

// ========== 2. TYPE DEFINITIONS ==========
typedef struct {
    int page;
    int cylinder;
    bool serviced;
} DiskRequest;

// ========== 3. HELPER FUNCTIONS ==========
int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// ========== 4. DISK SCHEDULING FUNCTIONS (PASTE ALL HERE) ==========
// Copy EVERY disk function from the library I provided earlier
// disk_fcfs, disk_sstf, disk_lstf, disk_lcfs, disk_scan, disk_cscan, disk_look, disk_clook
// (I'll show one as example - YOU PASTE THE REST)

void disk_fcfs(DiskRequest req[], int n, int head, int *page_seq) {
    for (int i = 0; i < n; i++) page_seq[i] = req[i].page;
}

void disk_sstf(DiskRequest req[], int n, int head, int *page_seq) {
    int curr = head;
    for (int cnt = 0; cnt < n; cnt++) {
        int min_dist = INT_MAX, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!req[i].serviced && abs(req[i].cylinder - curr) < min_dist) {
                min_dist = abs(req[i].cylinder - curr);
                idx = i;
            }
        }
        req[idx].serviced = true;
        curr = req[idx].cylinder;
        page_seq[cnt] = req[idx].page;
    }
}

void disk_lstf(DiskRequest req[], int n, int head, int *page_seq) {
    int curr = head;
    for (int cnt = 0; cnt < n; cnt++) {
        int max_dist = -1, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!req[i].serviced && abs(req[i].cylinder - curr) > max_dist) {
                max_dist = abs(req[i].cylinder - curr);
                idx = i;
            }
        }
        req[idx].serviced = true;
        curr = req[idx].cylinder;
        page_seq[cnt] = req[idx].page;
    }
}

void disk_lcfs(DiskRequest req[], int n, int head, int *page_seq) {
    for (int i = 0; i < n; i++) {
        page_seq[i] = req[n - 1 - i].page;
    }
}

void disk_scan(DiskRequest req[], int n, int head, int disk_size, int direction, int *page_seq) {
    int *cyl = (int*)malloc((n + 3) * sizeof(int));
    for (int i = 0; i < n; i++) cyl[i] = req[i].cylinder;
    cyl[n] = head; cyl[n+1] = 0; cyl[n+2] = disk_size;
    qsort(cyl, n + 3, sizeof(int), compare_int);
    int pos = 0; while (cyl[pos] != head) pos++;
    int idx = 0;
    if (direction == 1) {
        for (int i = pos + 1; i < n + 3; i++)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
        for (int i = pos - 1; i >= 0; i--)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    } else {
        for (int i = pos - 1; i >= 0; i--)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
        for (int i = pos + 1; i < n + 3; i++)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    }
    free(cyl);
}

void disk_cscan(DiskRequest req[], int n, int head, int disk_size, int *page_seq) {
    int *cyl = (int*)malloc((n + 3) * sizeof(int));
    for (int i = 0; i < n; i++) cyl[i] = req[i].cylinder;
    cyl[n] = head; cyl[n+1] = 0; cyl[n+2] = disk_size;
    qsort(cyl, n + 3, sizeof(int), compare_int);
    int pos = 0; while (cyl[pos] != head) pos++;
    int idx = 0;
    for (int i = pos + 1; i < n + 3; i++)
        for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
            { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    for (int i = 0; i < pos; i++)
        for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
            { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    free(cyl);
}

void disk_look(DiskRequest req[], int n, int head, int direction, int *page_seq) {
    int *cyl = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n; i++) cyl[i] = req[i].cylinder;
    cyl[n] = head;
    qsort(cyl, n + 1, sizeof(int), compare_int);
    int pos = 0; while (cyl[pos] != head) pos++;
    int idx = 0;
    if (direction == 1) {
        for (int i = pos + 1; i <= n; i++)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
        for (int i = pos - 1; i >= 0; i--)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    } else {
        for (int i = pos - 1; i >= 0; i--)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
        for (int i = pos + 1; i <= n; i++)
            for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
                { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    }
    free(cyl);
}

void disk_clook(DiskRequest req[], int n, int head, int *page_seq) {
    int *cyl = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n; i++) cyl[i] = req[i].cylinder;
    cyl[n] = head;
    qsort(cyl, n + 1, sizeof(int), compare_int);
    int pos = 0; while (cyl[pos] != head) pos++;
    int idx = 0;
    for (int i = pos + 1; i <= n; i++)
        for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
            { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    for (int i = 0; i < pos; i++)
        for (int j = 0; j < n; j++) if (req[j].cylinder == cyl[i] && !req[j].serviced)
            { req[j].serviced = true; page_seq[idx++] = req[j].page; }
    free(cyl);
}

// ========== 5. PAGE REPLACEMENT FUNCTIONS (PASTE ALL HERE) ==========
// Copy EVERY page replacement function from the library
// page_fifo, page_optimal, page_lru, page_mru, page_lfu, page_mfu

void page_fifo(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) f[i] = -1;
    int faults = 0, ptr = 0;
    printf("\n--- FIFO Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; break; }
        printf("%d\t", pages[i]);
        if (!found) { f[ptr] = pages[i]; ptr = (ptr + 1) % frames; faults++; }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f);
}

void page_optimal(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) f[i] = -1;
    int faults = 0;
    printf("\n--- Optimal Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; break; }
        printf("%d\t", pages[i]);
        if (!found) {
            int empty = -1;
            for (int j = 0; j < frames; j++) if (f[j] == -1) { empty = j; break; }
            if (empty != -1) f[empty] = pages[i];
            else {
                int replace = -1, farthest = -1;
                for (int j = 0; j < frames; j++) {
                    int next = INT_MAX;
                    for (int k = i+1; k < n; k++) if (pages[k] == f[j]) { next = k; break; }
                    if (next > farthest) { farthest = next; replace = j; }
                }
                f[replace] = pages[i];
            }
            faults++;
        }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f);
}

void page_lru(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    int *t = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) { f[i] = -1; t[i] = 0; }
    int faults = 0, timer = 0;
    printf("\n--- LRU Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false; int pos = -1;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; pos = j; break; }
        printf("%d\t", pages[i]);
        if (found) { t[pos] = timer++; }
        else {
            int empty = -1;
            for (int j = 0; j < frames; j++) if (f[j] == -1) { empty = j; break; }
            if (empty != -1) { f[empty] = pages[i]; t[empty] = timer++; }
            else {
                int lru = 0;
                for (int j = 1; j < frames; j++) if (t[j] < t[lru]) lru = j;
                f[lru] = pages[i]; t[lru] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f); free(t);
}

void page_mru(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    int *t = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) { f[i] = -1; t[i] = 0; }
    int faults = 0, timer = 0;
    printf("\n--- MRU Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false; int pos = -1;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; pos = j; break; }
        printf("%d\t", pages[i]);
        if (found) { t[pos] = timer++; }
        else {
            int empty = -1;
            for (int j = 0; j < frames; j++) if (f[j] == -1) { empty = j; break; }
            if (empty != -1) { f[empty] = pages[i]; t[empty] = timer++; }
            else {
                int mru = 0;
                for (int j = 1; j < frames; j++) if (t[j] > t[mru]) mru = j;
                f[mru] = pages[i]; t[mru] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f); free(t);
}

void page_lfu(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    int *freq = (int*)calloc(frames, sizeof(int));
    int *load = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) f[i] = -1;
    int faults = 0, timer = 0;
    printf("\n--- LFU Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false; int pos = -1;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; pos = j; break; }
        printf("%d\t", pages[i]);
        if (found) { freq[pos]++; }
        else {
            int empty = -1;
            for (int j = 0; j < frames; j++) if (f[j] == -1) { empty = j; break; }
            if (empty != -1) { f[empty] = pages[i]; freq[empty] = 1; load[empty] = timer++; }
            else {
                int lfu = 0;
                for (int j = 1; j < frames; j++) 
                    if (freq[j] < freq[lfu] || (freq[j] == freq[lfu] && load[j] < load[lfu])) lfu = j;
                f[lfu] = pages[i]; freq[lfu] = 1; load[lfu] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f); free(freq); free(load);
}

void page_mfu(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    int *freq = (int*)calloc(frames, sizeof(int));
    int *load = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) f[i] = -1;
    int faults = 0, timer = 0;
    printf("\n--- MFU Page Replacement ---\n");
    printf("Ref\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false; int pos = -1;
        for (int j = 0; j < frames; j++) if (f[j] == pages[i]) { found = true; pos = j; break; }
        printf("%d\t", pages[i]);
        if (found) { freq[pos]++; }
        else {
            int empty = -1;
            for (int j = 0; j < frames; j++) if (f[j] == -1) { empty = j; break; }
            if (empty != -1) { f[empty] = pages[i]; freq[empty] = 1; load[empty] = timer++; }
            else {
                int mfu = 0;
                for (int j = 1; j < frames; j++) 
                    if (freq[j] > freq[mfu] || (freq[j] == freq[mfu] && load[j] < load[mfu])) mfu = j;
                f[mfu] = pages[i]; freq[mfu] = 1; load[mfu] = timer++;
            }
            faults++;
        }
        for (int j = 0; j < frames; j++) printf("%s%d", j?" ":"", f[j]==-1?-1:f[j]);
        printf("\t\t%s\n", found?"Hit":"Fault");
    }
    printf("Total Faults: %d, Hit Ratio: %.2f%%\n", faults, (float)(n-faults)/n*100);
    free(f); free(freq); free(load);
}

// ========== 6. MAIN DRIVER (MODIFY SELECTION HERE) ==========
int main() {
    int frames, head, disk_size, direction;
    char disk_algo[20], page_algo[20];
    DiskRequest req[MAX_INPUT];
    int n = 0;

    printf("\n========== DISK + PAGE REPLACEMENT COMBINATION ==========\n");
    printf("Enter number of frames: "); scanf("%d", &frames);
    
    printf("\nAvailable disk algorithms: fcfs, sstf, lstf, lcfs, scan, cscan, look, clook\n");
    printf("Enter disk scheduling algorithm: "); scanf("%s", disk_algo);
    
    printf("Enter initial head position: "); scanf("%d", &head);
    
    if (strcmp(disk_algo, "scan") == 0 || strcmp(disk_algo, "cscan") == 0) {
        printf("Enter disk size (max cylinder): "); scanf("%d", &disk_size);
    }
    if (strcmp(disk_algo, "scan") == 0 || strcmp(disk_algo, "look") == 0) {
        printf("Enter direction (1=right, 0=left): "); scanf("%d", &direction);
    }
    
    printf("\nAvailable page algorithms: fifo, optimal, lru, mru, lfu, mfu\n");
    printf("Enter page replacement algorithm: "); scanf("%s", page_algo);
    
    printf("\nEnter pairs (page cylinder) and -1 to end:\n");
    while (1) {
        int p, c; 
        scanf("%d", &p); 
        if (p == -1) break;
        scanf("%d", &c);
        req[n].page = p; 
        req[n].cylinder = c; 
        req[n].serviced = false; 
        n++;
    }

    int *page_seq = (int*)malloc(n * sizeof(int));

    // === SELECT DISK ALGORITHM ===
    if (strcmp(disk_algo, "fcfs") == 0) disk_fcfs(req, n, head, page_seq);
    else if (strcmp(disk_algo, "sstf") == 0) disk_sstf(req, n, head, page_seq);
    else if (strcmp(disk_algo, "lstf") == 0) disk_lstf(req, n, head, page_seq);
    else if (strcmp(disk_algo, "lcfs") == 0) disk_lcfs(req, n, head, page_seq);
    else if (strcmp(disk_algo, "scan") == 0) disk_scan(req, n, head, disk_size, direction, page_seq);
    else if (strcmp(disk_algo, "cscan") == 0) disk_cscan(req, n, head, disk_size, page_seq);
    else if (strcmp(disk_algo, "look") == 0) disk_look(req, n, head, direction, page_seq);
    else if (strcmp(disk_algo, "clook") == 0) disk_clook(req, n, head, page_seq);
    else { printf("Invalid disk algorithm.\n"); return 1; }

    printf("\nGenerated Page Reference String: ");
    for (int i = 0; i < n; i++) printf("%d ", page_seq[i]);
    printf("\n");

    // === SELECT PAGE REPLACEMENT ALGORITHM ===
    if (strcmp(page_algo, "fifo") == 0) page_fifo(page_seq, n, frames);
    else if (strcmp(page_algo, "optimal") == 0) page_optimal(page_seq, n, frames);
    else if (strcmp(page_algo, "lru") == 0) page_lru(page_seq, n, frames);
    else if (strcmp(page_algo, "mru") == 0) page_mru(page_seq, n, frames);
    else if (strcmp(page_algo, "lfu") == 0) page_lfu(page_seq, n, frames);
    else if (strcmp(page_algo, "mfu") == 0) page_mfu(page_seq, n, frames);
    else { printf("Invalid page algorithm.\n"); return 1; }

    free(page_seq);
    return 0;
}