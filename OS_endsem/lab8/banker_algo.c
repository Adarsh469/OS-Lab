/**
 * Banker's Algorithm - Deadlock Avoidance
 * Lab 8 Exercise 1
 * 
 * Features:
 * - User inputs number of processes and resources
 * - User inputs Allocation, Max, Available
 * - Computes and displays Need matrix
 * - Checks initial safety and prints safe sequence
 * - Processes resource requests and updates state if safe
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_P 20
#define MAX_R 20

int n, m;                               // number of processes and resources
int Allocation[MAX_P][MAX_R];
int Max[MAX_P][MAX_R];
int Need[MAX_P][MAX_R];
int Available[MAX_R];

// Function prototypes
void computeNeed();
bool isSafe(int safeSeq[]);
bool requestResources(int pid, int req[]);
void printMatrix(int mat[MAX_P][MAX_R], const char *name);
void printVector(int vec[MAX_R], const char *name);

int main() {
    printf("\n========== BANKER'S ALGORITHM ==========\n\n");
    
    // Input number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    
    // Input Allocation matrix
    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }
    
    // Input Max matrix
    printf("\nEnter Max Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Max[i][j]);
        }
    }
    
    // Input Available vector
    printf("\nEnter Available Resources (vector of size %d):\n", m);
    for (int j = 0; j < m; j++) {
        scanf("%d", &Available[j]);
    }
    
    // Compute Need matrix
    computeNeed();
    printMatrix(Need, "Need Matrix");
    
    // Initial safety check
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
        for (int i = 0; i < n; i++) {
            printf("P%d", safeSeq[i]);
            if (i < n - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state.\n");
    }
    
    // Process resource requests
    printf("\n--- Resource Request Handling ---\n");
    printf("Enter requests in format: <pid> <r1> <r2> ... <rm>\n");
    printf("Enter -1 to exit.\n");
    
    while (1) {
        int pid;
        printf("\nEnter request: ");
        scanf("%d", &pid);
        if (pid == -1) break;
        
        if (pid < 0 || pid >= n) {
            printf("Invalid process ID. Try again.\n");
            continue;
        }
        
        int req[MAX_R];
        for (int j = 0; j < m; j++) {
            scanf("%d", &req[j]);
        }
        
        if (requestResources(pid, req)) {
            printf("Request GRANTED.\n");
            printf("Updated Available: ");
            printVector(Available, "");
            printMatrix(Allocation, "Updated Allocation");
            printMatrix(Need, "Updated Need");
            
            int seq[MAX_P];
            if (isSafe(seq)) {
                printf("System remains SAFE. Sequence: ");
                for (int i = 0; i < n; i++) printf("P%d ", seq[i]);
                printf("\n");
            }
        } else {
            printf("Request DENIED (unsafe or invalid).\n");
        }
    }
    
    return 0;
}

void computeNeed() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}

bool isSafe(int safeSeq[]) {
    int Work[MAX_R];
    bool Finish[MAX_P] = {false};
    
    // Initialize Work = Available
    for (int j = 0; j < m; j++) {
        Work[j] = Available[j];
    }
    
    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    // Process i can finish
                    for (int j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }
                    Finish[i] = true;
                    safeSeq[count++] = i;
                    found = true;
                }
            }
        }
        if (!found) {
            // No process could be found; unsafe
            return false;
        }
    }
    return true;
}

bool requestResources(int pid, int req[]) {
    // Step 1: Check request <= Need
    for (int j = 0; j < m; j++) {
        if (req[j] > Need[pid][j]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }
    
    // Step 2: Check request <= Available
    for (int j = 0; j < m; j++) {
        if (req[j] > Available[j]) {
            printf("Process must wait: resources not available.\n");
            return false;
        }
    }
    
    // Step 3: Tentatively allocate
    for (int j = 0; j < m; j++) {
        Available[j] -= req[j];
        Allocation[pid][j] += req[j];
        Need[pid][j] -= req[j];
    }
    
    // Step 4: Check safety
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        return true;  // Keep changes
    } else {
        // Rollback
        for (int j = 0; j < m; j++) {
            Available[j] += req[j];
            Allocation[pid][j] -= req[j];
            Need[pid][j] += req[j];
        }
        return false;
    }
}

void printMatrix(int mat[MAX_P][MAX_R], const char *name) {
    printf("\n%s:\n", name);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void printVector(int vec[MAX_R], const char *name) {
    if (name[0] != '\0') printf("%s: ", name);
    for (int j = 0; j < m; j++) {
        printf("%d ", vec[j]);
    }
    printf("\n");
}