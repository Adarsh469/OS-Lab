/**
 * Question 3: Memory Allocation (Sort Blocks Ascending + Best Fit) + Safety Check
 * "NextFitIncreasing" interpretation: sort free blocks ascending then Best Fit.
 * Safety Check: Banker's algorithm adapted for memory blocks.
 * Compile: gcc mem_alloc_safety.c -o mem_alloc_safety
 * Run: ./mem_alloc_safety
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROC 20
#define MAX_BLOCK 20

typedef struct {
    int size;           // current free size
    int original;       // original size (for display)
    int allocated_to;   // process id or -1
} MemoryBlock;

typedef struct {
    int max;            // maximum memory needed
    int allocated;      // currently allocated
    int need;           // max - allocated
} Process;

int nproc, nblock;
MemoryBlock blocks[MAX_BLOCK];
Process procs[MAX_PROC];

int compare_asc(const void *a, const void *b) {
    return ((MemoryBlock*)a)->size - ((MemoryBlock*)b)->size;
}

// Display current memory blocks (free holes)
void display_blocks(const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1)
            printf("%d ", blocks[i].size);
    }
    printf("\n");
}

// Safety algorithm (Banker's for memory)
bool is_safe(int safe_seq[]) {
    // Work = available hole sizes (copy)
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
                // Can process i fit into any available hole?
                for (int j = 0; j < work_count; j++) {
                    if (work[j] >= procs[i].need) {
                        // Process can finish, release its allocated block
                        // Find block that was allocated to this process
                        for (int k = 0; k < nblock; k++) {
                            if (blocks[k].allocated_to == i) {
                                // Add its size back to work
                                work[work_count++] = blocks[k].original; // release original block size
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
                // Already has all memory, can finish
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

// Best Fit allocation on blocks sorted ascending
bool allocate_best_fit(int pid, int request) {
    if (request > procs[pid].need) {
        printf("Error: Request exceeds maximum need.\n");
        return false;
    }
    
    // Find best fit among free blocks
    int best_idx = -1;
    int min_remainder = __INT_MAX__;
    for (int i = 0; i < nblock; i++) {
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request) {
            int remain = blocks[i].size - request;
            if (remain < min_remainder) {
                min_remainder = remain;
                best_idx = i;
            }
        }
    }
    if (best_idx == -1) {
        printf("Request cannot be satisfied: no sufficient memory.\n");
        return false;
    }
    
    // Tentatively allocate
    blocks[best_idx].size -= request;
    blocks[best_idx].allocated_to = pid;
    procs[pid].allocated += request;
    procs[pid].need -= request;
    
    // Check safety
    int safe_seq[MAX_PROC];
    if (is_safe(safe_seq)) {
        printf("Safe | Seq: ");
        for (int i = 0; i < nproc; i++) {
            if (procs[safe_seq[i]].need >= 0) // valid
                printf("P%d ", safe_seq[i]);
        }
        printf("\n");
        return true;
    } else {
        // Rollback
        blocks[best_idx].size += request;
        blocks[best_idx].allocated_to = -1;
        procs[pid].allocated -= request;
        procs[pid].need += request;
        printf("Unsafe request denied.\n");
        return false;
    }
}

int main() {
    printf("\n========== MEMORY ALLOCATION + SAFETY CHECK ==========\n");
    printf("Enter number of processes: ");
    scanf("%d", &nproc);
    printf("Enter number of memory blocks: ");
    scanf("%d", &nblock);
    
    printf("Enter sizes of %d memory blocks:\n", nblock);
    for (int i = 0; i < nblock; i++) {
        scanf("%d", &blocks[i].size);
        blocks[i].original = blocks[i].size;
        blocks[i].allocated_to = -1;
    }
    
    printf("Enter maximum memory required for each process:\n");
    for (int i = 0; i < nproc; i++) {
        printf("P%d: ", i);
        scanf("%d", &procs[i].max);
        procs[i].allocated = 0;
        procs[i].need = procs[i].max;
    }
    
    // Initial safety check
    int safe_seq[MAX_PROC];
    if (!is_safe(safe_seq)) {
        printf("Initial state is unsafe. Exiting.\n");
        return 1;
    }
    printf("Initial state is safe.\n");
    
    printf("\nEnter requests in format: pid memory (-1 to end):\n");
    while (1) {
        int pid, req;
        scanf("%d", &pid);
        if (pid == -1) break;
        scanf("%d", &req);
        
        printf("\nP%d request %d\n", pid, req);
        display_blocks("Before");
        
        // Sort free blocks ascending before allocation (NextFitIncreasing)
        qsort(blocks, nblock, sizeof(MemoryBlock), compare_asc);
        // Note: sorting changes block order, but we don't preserve original ordering.
        // For display, we show the sorted free blocks.
        
        bool granted = allocate_best_fit(pid, req);
        if (granted) {
            display_blocks("After");
        } else {
            printf("After: (no change)\n");
        }
    }
    
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < nproc; i++) {
        printf("P%d: %d\n", i, procs[i].allocated);
    }
    
    return 0;
}