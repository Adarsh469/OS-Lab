// ========== PAGE REPLACEMENT FUNCTIONS ==========

void page_fifo(int pages[], int n, int frames) {
    int *f = (int*)malloc(frames * sizeof(int));
    for (int i = 0; i < frames; i++) f[i] = -1;
    int faults = 0, ptr = 0;
    printf("\nRef\tFrames\t\tFault?\n");
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
    printf("\nRef\tFrames\t\tFault?\n");
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
    printf("\nRef\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
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
    printf("\nRef\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
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
    printf("\nRef\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
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
    printf("\nRef\tFrames\t\tFault?\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        int pos = -1;
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