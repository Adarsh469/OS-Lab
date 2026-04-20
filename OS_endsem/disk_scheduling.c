// ========== DISK SCHEDULING FUNCTIONS ==========

typedef struct {
    int page;
    int cylinder;
    bool serviced;
} DiskRequest;

// FCFS
void disk_fcfs(DiskRequest req[], int n, int head, int *page_seq) {
    for (int i = 0; i < n; i++) page_seq[i] = req[i].page;
}

// SSTF
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

// LSTF (Largest Seek Time First)
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

// LCFS (Last Come First Serve) - service in reverse arrival order
void disk_lcfs(DiskRequest req[], int n, int head, int *page_seq) {
    for (int i = 0; i < n; i++) {
        page_seq[i] = req[n - 1 - i].page;
    }
}

// SCAN (Elevator)
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

// C-SCAN
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

// LOOK
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

// C-LOOK
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