/**
 * MEMORY ALLOCATION + SAFETY CHECK - PLUG AND PLAY FRAMEWORK
 * Compile: gcc mem_alloc_safety.c -o mem_alloc_safety
 * Run: ./mem_alloc_safety
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MAX_PROC 20
#define MAX_BLOCK 20

// ========== TYPE DEFINITIONS ==========
typedef struct {
    int size;           // current free size
    int original;       // original size
    int allocated_to;   // -1 if free
} MemoryBlock;

typedef struct {
    int max;
    int allocated;
    int need;
} Process;

// ========== HELPER COMPARE FUNCTIONS ==========
int compare_asc(const void *a, const void *b) {
    return ((MemoryBlock*)a)->size - ((MemoryBlock*)b)->size;
}

int compare_desc(const void *a, const void *b) {
    return ((MemoryBlock*)b)->size - ((MemoryBlock*)a)->size;
}

// ========== ALLOCATION STRATEGIES ==========

// First Fit
int first_fit(MemoryBlock blocks[], int nblock, int request) {
    for (int i = 0; i < nblock; i++)
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request)
            return i;
    return -1;
}

// Best Fit
int best_fit(MemoryBlock blocks[], int nblock, int request) {
    int best = -1, min_rem = INT_MAX;
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request) {
            int rem = blocks[i].size - request;
            if (rem < min_rem) { min_rem = rem; best = i; }
        }
    }
    return best;
}

// Worst Fit
int worst_fit(MemoryBlock blocks[], int nblock, int request) {
    int worst = -1, max_rem = -1;
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request) {
            int rem = blocks[i].size - request;
            if (rem > max_rem) { max_rem = rem; worst = i; }
        }
    }
    return worst;
}

// Next Fit
int last_index = 0;
int next_fit(MemoryBlock blocks[], int nblock, int request) {
    for (int cnt = 0; cnt < nblock; cnt++) {
        int i = (last_index + cnt) % nblock;
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request) {
            last_index = i;
            return i;
        }
    }
    return -1;
}

// ========== SORTING FUNCTIONS ==========
void sort_asc(MemoryBlock blocks[], int n) {
    qsort(blocks, n, sizeof(MemoryBlock), compare_asc);
}
void sort_desc(MemoryBlock blocks[], int n) {
    qsort(blocks, n, sizeof(MemoryBlock), compare_desc);
}
void no_sort(MemoryBlock blocks[], int n) {
    // keep original order - do nothing
}

// ========== SAFETY CHECK (BANKER'S FOR MEMORY) ==========
bool is_safe(MemoryBlock blocks[], int nblock, Process procs[], int nproc, int safe_seq[]) {
    // Work = available hole sizes
    int *work = (int*)malloc(nblock * sizeof(int));
    int work_count = 0;
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1)
            work[work_count++] = blocks[i].size;
    }
    bool *finish = (bool*)calloc(nproc, sizeof(bool));
    int seq_idx = 0;
    
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < nproc; i++) {
            if (!finish[i] && procs[i].need > 0) {
                for (int j = 0; j < work_count; j++) {
                    if (work[j] >= procs[i].need) {
                        // Process finishes, release its allocated block
                        for (int k = 0; k < nblock; k++) {
                            if (blocks[k].allocated_to == i) {
                                work[work_count++] = blocks[k].original;
                                break;
                            }
                        }
                        finish[i] = true;
                        safe_seq[seq_idx++] = i;
                        changed = 1;
                        break;
                    }
                }
            } else if (!finish[i] && procs[i].need == 0) {
                for (int k = 0; k < nblock; k++) {
                    if (blocks[k].allocated_to == i) {
                        work[work_count++] = blocks[k].original;
                        break;
                    }
                }
                finish[i] = true;
                safe_seq[seq_idx++] = i;
                changed = 1;
            }
        }
    } while (changed);
    
    free(work);
    bool safe = true;
    for (int i = 0; i < nproc; i++) if (!finish[i]) { safe = false; break; }
    free(finish);
    return safe;
}

// ========== DISPLAY FUNCTIONS ==========
void display_blocks(MemoryBlock blocks[], int nblock, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1)
            printf("%d ", blocks[i].size);
    }
    printf("\n");
}

// ========== MAIN DRIVER ==========
int main() {
    int nproc, nblock;
    MemoryBlock blocks[MAX_BLOCK];
    Process procs[MAX_PROC];
    char alloc_strat[20], sort_opt[20];
    
    printf("\n========== MEMORY ALLOCATION + SAFETY CHECK ==========\n");
    printf("Enter number of processes: "); scanf("%d", &nproc);
    printf("Enter number of memory blocks: "); scanf("%d", &nblock);
    
    printf("Enter sizes of %d memory blocks:\n", nblock);
    for (int i = 0; i < nblock; i++) {
        scanf("%d", &blocks[i].size);
        blocks[i].original = blocks[i].size;
        blocks[i].allocated_to = -1;
    }
    
    printf("Enter maximum memory required for each process:\n");
    for (int i = 0; i < nproc; i++) {
        printf("P%d: ", i); scanf("%d", &procs[i].max);
        procs[i].allocated = 0;
        procs[i].need = procs[i].max;
    }
    
    printf("\nAvailable allocation strategies: first, best, worst, next\n");
    printf("Enter allocation strategy: "); scanf("%s", alloc_strat);
    printf("Available sorting: asc, desc, none\n");
    printf("Enter sorting option: "); scanf("%s", sort_opt);
    
    // Set function pointers
    int (*alloc_func)(MemoryBlock[], int, int) = NULL;
    void (*sort_func)(MemoryBlock[], int) = NULL;
    
    if (strcmp(alloc_strat, "first") == 0) alloc_func = first_fit;
    else if (strcmp(alloc_strat, "best") == 0) alloc_func = best_fit;
    else if (strcmp(alloc_strat, "worst") == 0) alloc_func = worst_fit;
    else if (strcmp(alloc_strat, "next") == 0) alloc_func = next_fit;
    else { printf("Invalid allocation strategy.\n"); return 1; }
    
    if (strcmp(sort_opt, "asc") == 0) sort_func = sort_asc;
    else if (strcmp(sort_opt, "desc") == 0) sort_func = sort_desc;
    else sort_func = no_sort;
    
    // Initial safety check
    int safe_seq[MAX_PROC];
    if (!is_safe(blocks, nblock, procs, nproc, safe_seq)) {
        printf("Initial state is UNSAFE. Exiting.\n");
        return 1;
    }
    printf("Initial state is SAFE.\n");
    
    printf("\nEnter requests in format: pid memory (-1 to end):\n");
    while (1) {
        int pid, req;
        scanf("%d", &pid);
        if (pid == -1) break;
        scanf("%d", &req);
        
        printf("\nP%d request %d\n", pid, req);
        display_blocks(blocks, nblock, "Before");
        
        // Apply sorting
        sort_func(blocks, nblock);
        
        // Find block using selected strategy
        int idx = alloc_func(blocks, nblock, req);
        
        if (idx == -1) {
            printf("Request cannot be satisfied.\n");
            continue;
        }
        if (req > procs[pid].need) {
            printf("Error: Request exceeds maximum need.\n");
            continue;
        }
        
        // Tentative allocation
        blocks[idx].size -= req;
        blocks[idx].allocated_to = pid;
        procs[pid].allocated += req;
        procs[pid].need -= req;
        
        // Check safety
        if (is_safe(blocks, nblock, procs, nproc, safe_seq)) {
            printf("After");
            display_blocks(blocks, nblock, "");
            printf("Safe | Seq: ");
            for (int i = 0; i < nproc; i++) printf("P%d ", safe_seq[i]);
            printf("\n");
        } else {
            // Rollback
            blocks[idx].size += req;
            blocks[idx].allocated_to = -1;
            procs[pid].allocated -= req;
            procs[pid].need += req;
            printf("Unsafe request denied.\n");
        }
    }
    
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < nproc; i++) {
        printf("P%d: %d\n", i, procs[i].allocated);
    }
    
    return 0;
}