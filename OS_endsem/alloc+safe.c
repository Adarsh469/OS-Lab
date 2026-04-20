int main() {
    // ... read inputs ...
    printf("Select allocation strategy (first/best/worst/next): ");
    scanf("%s", alloc_strat);
    printf("Select sorting (asc/desc/none): ");
    scanf("%s", sort_opt);

    // Function pointers
    int (*alloc_func)(MemoryBlock[], int, int) = NULL;
    void (*sort_func)(MemoryBlock[], int) = NULL;

    if (strcmp(alloc_strat, "first") == 0) alloc_func = first_fit;
    else if (strcmp(alloc_strat, "best") == 0) alloc_func = best_fit;
    else if (strcmp(alloc_strat, "worst") == 0) alloc_func = worst_fit;
    else if (strcmp(alloc_strat, "next") == 0) alloc_func = next_fit;

    if (strcmp(sort_opt, "asc") == 0) sort_func = sort_asc;
    else if (strcmp(sort_opt, "desc") == 0) sort_func = sort_desc;
    else sort_func = no_sort;

    while (1) {
        // read request
        sort_func(blocks, nblock);
        int idx = alloc_func(blocks, nblock, request);
        // ... allocate, check safety, rollback if unsafe ...
    }
}