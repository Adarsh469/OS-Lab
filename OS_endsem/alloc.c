// ========== MEMORY ALLOCATION STRATEGIES ==========

// First Fit
int first_fit(MemoryBlock blocks[], int nblock, int request) {
    for (int i = 0; i < nblock; i++)
        if (blocks[i].allocated_to == -1 && blocks[i].size >= request) return i;
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

// Next Fit (global last index)
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

void sort_asc(MemoryBlock blocks[], int n) {
    qsort(blocks, n, sizeof(MemoryBlock), compare_asc);
}
void sort_desc(MemoryBlock blocks[], int n) {
    qsort(blocks, n, sizeof(MemoryBlock), compare_desc);
}
void no_sort(MemoryBlock blocks[], int n) { /* keep original */ }