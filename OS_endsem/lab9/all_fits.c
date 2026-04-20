/**
 * Lab 9 Exercise 1: Memory Allocation Strategies
 * Fully user-input driven
 * Compile: gcc memory_alloc.c -o memory_alloc
 * Run: ./memory_alloc
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX 50

typedef struct {
    int original;
    int remaining;
    int allocated_to;   // -1 if free
} Partition;

typedef struct {
    int id;
    int size;
    int allocated_partition;
    bool allocated;
} Process;

// Function prototypes
void reset(Partition partitions[], int np, Process processes[], int nprocs);
void firstFit(Partition partitions[], int np, Process processes[], int nprocs);
void bestFit(Partition partitions[], int np, Process processes[], int nprocs);
void worstFit(Partition partitions[], int np, Process processes[], int nprocs);
void printResults(Partition partitions[], int np, Process processes[], int nprocs, const char* algo);

int main() {
    Partition partitions[MAX];
    Process processes[MAX];
    int np, nprocs;

    printf("\n========== MEMORY ALLOCATION STRATEGIES ==========\n\n");

    // Input partitions
    printf("Enter number of memory partitions: ");
    scanf("%d", &np);
    if (np <= 0 || np > MAX) {
        printf("Invalid number of partitions. Exiting.\n");
        return 1;
    }
    printf("Enter sizes of %d partitions (in KB):\n", np);
    for (int i = 0; i < np; i++) {
        printf("Partition %d: ", i);
        scanf("%d", &partitions[i].original);
        partitions[i].remaining = partitions[i].original;
        partitions[i].allocated_to = -1;
    }

    // Input processes
    printf("\nEnter number of processes: ");
    scanf("%d", &nprocs);
    if (nprocs <= 0 || nprocs > MAX) {
        printf("Invalid number of processes. Exiting.\n");
        return 1;
    }
    printf("Enter memory requirements of %d processes (in KB):\n", nprocs);
    for (int i = 0; i < nprocs; i++) {
        printf("Process P%d: ", i);
        scanf("%d", &processes[i].size);
        processes[i].id = i;
        processes[i].allocated = false;
        processes[i].allocated_partition = -1;
    }

    // Run First-Fit
    printf("\n--- FIRST FIT ---\n");
    reset(partitions, np, processes, nprocs);
    firstFit(partitions, np, processes, nprocs);
    printResults(partitions, np, processes, nprocs, "First-Fit");

    // Run Best-Fit
    printf("\n--- BEST FIT ---\n");
    reset(partitions, np, processes, nprocs);
    bestFit(partitions, np, processes, nprocs);
    printResults(partitions, np, processes, nprocs, "Best-Fit");

    // Run Worst-Fit
    printf("\n--- WORST FIT ---\n");
    reset(partitions, np, processes, nprocs);
    worstFit(partitions, np, processes, nprocs);
    printResults(partitions, np, processes, nprocs, "Worst-Fit");

    return 0;
}

void reset(Partition partitions[], int np, Process processes[], int nprocs) {
    for (int i = 0; i < np; i++) {
        partitions[i].remaining = partitions[i].original;
        partitions[i].allocated_to = -1;
    }
    for (int i = 0; i < nprocs; i++) {
        processes[i].allocated = false;
        processes[i].allocated_partition = -1;
    }
}

void firstFit(Partition partitions[], int np, Process processes[], int nprocs) {
    for (int i = 0; i < nprocs; i++) {
        for (int j = 0; j < np; j++) {
            if (partitions[j].allocated_to == -1 && partitions[j].remaining >= processes[i].size) {
                partitions[j].allocated_to = i;
                partitions[j].remaining -= processes[i].size;
                processes[i].allocated = true;
                processes[i].allocated_partition = j;
                break;
            }
        }
    }
}

void bestFit(Partition partitions[], int np, Process processes[], int nprocs) {
    for (int i = 0; i < nprocs; i++) {
        int best_idx = -1;
        int min_remainder = INT_MAX;
        for (int j = 0; j < np; j++) {
            if (partitions[j].allocated_to == -1 && partitions[j].remaining >= processes[i].size) {
                int remainder = partitions[j].remaining - processes[i].size;
                if (remainder < min_remainder) {
                    min_remainder = remainder;
                    best_idx = j;
                }
            }
        }
        if (best_idx != -1) {
            partitions[best_idx].allocated_to = i;
            partitions[best_idx].remaining -= processes[i].size;
            processes[i].allocated = true;
            processes[i].allocated_partition = best_idx;
        }
    }
}

void worstFit(Partition partitions[], int np, Process processes[], int nprocs) {
    for (int i = 0; i < nprocs; i++) {
        int worst_idx = -1;
        int max_remainder = -1;
        for (int j = 0; j < np; j++) {
            if (partitions[j].allocated_to == -1 && partitions[j].remaining >= processes[i].size) {
                int remainder = partitions[j].remaining - processes[i].size;
                if (remainder > max_remainder) {
                    max_remainder = remainder;
                    worst_idx = j;
                }
            }
        }
        if (worst_idx != -1) {
            partitions[worst_idx].allocated_to = i;
            partitions[worst_idx].remaining -= processes[i].size;
            processes[i].allocated = true;
            processes[i].allocated_partition = worst_idx;
        }
    }
}

void printResults(Partition partitions[], int np, Process processes[], int nprocs, const char* algo) {
    printf("\n%s Allocation:\n", algo);
    printf("Process\tSize\tAllocated Partition\tStatus\n");
    for (int i = 0; i < nprocs; i++) {
        printf("P%d\t%d\t", processes[i].id, processes[i].size);
        if (processes[i].allocated)
            printf("Partition %d (rem %d)\tAllocated\n", 
                   processes[i].allocated_partition, 
                   partitions[processes[i].allocated_partition].remaining);
        else
            printf("-\t\t\tNot Allocated\n");
    }

    printf("\nPartition Status:\n");
    printf("Partition\tOriginal\tRemaining\tAllocated to\n");
    int total_external = 0;
    for (int i = 0; i < np; i++) {
        printf("%d\t\t%d\t\t%d\t\t", i, partitions[i].original, partitions[i].remaining);
        if (partitions[i].allocated_to != -1)
            printf("P%d\n", partitions[i].allocated_to);
        else {
            printf("Free\n");
            total_external += partitions[i].remaining;
        }
    }
    printf("\nTotal External Fragmentation: %d KB\n", total_external);
}