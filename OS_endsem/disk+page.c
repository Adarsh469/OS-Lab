// ========== UNIFIED DRIVER: DISK + PAGE ==========
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define MAX_INPUT 100

// ... Include all disk and page functions here ...

int compare_int(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main() {
    int frames, head, disk_size, direction;
    char disk_algo[20], page_algo[20];
    DiskRequest req[MAX_INPUT];
    int n = 0;

    printf("Enter number of frames: "); scanf("%d", &frames);
    printf("Enter disk scheduling algorithm (fcfs/sstf/lstf/lcfs/scan/cscan/look/clook): ");
    scanf("%s", disk_algo);
    printf("Enter initial head position: "); scanf("%d", &head);
    if (strcmp(disk_algo, "scan") == 0 || strcmp(disk_algo, "cscan") == 0) {
        printf("Enter disk size (max cylinder): "); scanf("%d", &disk_size);
    }
    if (strcmp(disk_algo, "scan") == 0 || strcmp(disk_algo, "look") == 0) {
        printf("Enter direction (1=right, 0=left): "); scanf("%d", &direction);
    }
    printf("Enter page replacement algorithm (fifo/optimal/lru/mru/lfu/mfu): ");
    scanf("%s", page_algo);
    printf("Enter pairs (page cylinder) and -1 to end:\n");
    while (1) {
        int p, c; scanf("%d", &p); if (p == -1) break;
        scanf("%d", &c);
        req[n].page = p; req[n].cylinder = c; req[n].serviced = false; n++;
    }

    int *page_seq = (int*)malloc(n * sizeof(int));

    // Call selected disk algorithm
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

    // Call selected page replacement algorithm
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