/**
 * Deadlock Detection Algorithm
 * Lab 8 Exercise 2
 * 
 * Features:
 * - User inputs number of processes and resources
 * - User inputs Allocation, Request, Available
 * - Runs detection algorithm to find deadlocked processes
 * - Handles additional request scenario (e.g., P2 requests one more C)
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_P 20
#define MAX_R 20

int n, m;
int Allocation[MAX_P][MAX_R];
int Request[MAX_P][MAX_R];
int Available[MAX_R];

// Function prototypes
bool detectDeadlock(bool deadlockedProcs[]);
void printMatrix(int mat[MAX_P][MAX_R], const char *name);
void printVector(int vec[MAX_R], const char *name);

int main() {
    printf("\n========== DEADLOCK DETECTION ALGORITHM ==========\n\n");
    
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
    
    // Input Request matrix (current requests)
    printf("\nEnter Request Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Request[i][j]);
        }
    }
    
    // Input Available vector
    printf("\nEnter Available Resources (vector of size %d):\n", m);
    for (int j = 0; j < m; j++) {
        scanf("%d", &Available[j]);
    }
    
    // Initial deadlock check
    bool deadlocked[MAX_P] = {false};
    if (detectDeadlock(deadlocked)) {
        printf("\nSystem is DEADLOCKED.\nDeadlocked processes: ");
        bool any = false;
        for (int i = 0; i < n; i++) {
            if (deadlocked[i]) {
                printf("P%d ", i);
                any = true;
            }
        }
        if (!any) printf("None");
        printf("\n");
    } else {
        printf("\nSystem is NOT deadlocked.\n");
    }
    
    // Handle additional request (as per manual: "P2 make one additional request for instance of type C")
    printf("\n--- Simulate Additional Request ---\n");
    printf("Enter additional request in format: <pid> <r1> <r2> ... <rm>\n");
    printf("(Enter -1 for pid to skip): ");
    
    int pid;
    scanf("%d", &pid);
    if (pid != -1) {
        int addReq[MAX_R];
        for (int j = 0; j < m; j++) {
            scanf("%d", &addReq[j]);
        }
        
        // Add to existing Request
        for (int j = 0; j < m; j++) {
            Request[pid][j] += addReq[j];
        }
        
        printf("\nAfter additional request:\n");
        printMatrix(Request, "Updated Request Matrix");
        
        bool newDeadlocked[MAX_P] = {false};
        if (detectDeadlock(newDeadlocked)) {
            printf("\nSystem is now DEADLOCKED.\nDeadlocked processes: ");
            for (int i = 0; i < n; i++) {
                if (newDeadlocked[i]) printf("P%d ", i);
            }
            printf("\n");
        } else {
            printf("\nSystem remains DEADLOCK-FREE.\n");
        }
    }
    
    return 0;
}

bool detectDeadlock(bool deadlockedProcs[]) {
    int Work[MAX_R];
    bool Finish[MAX_P];
    
    // Step 1: Initialize Work = Available
    for (int j = 0; j < m; j++) {
        Work[j] = Available[j];
    }
    
    // Finish[i] = true if Allocation[i] == 0
    for (int i = 0; i < n; i++) {
        bool allZero = true;
        for (int j = 0; j < m; j++) {
            if (Allocation[i][j] != 0) {
                allZero = false;
                break;
            }
        }
        Finish[i] = allZero;
    }
    
    // Step 2 & 3: Find an i such that !Finish[i] and Request[i] <= Work
    int count = 0;
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < m; j++) {
                    if (Request[i][j] > Work[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    // Process can finish; release its allocated resources
                    for (int j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }
                    Finish[i] = true;
                    changed = true;
                    count++;
                }
            }
        }
    } while (changed);
    
    // Step 4: Any process with Finish[i] == false is deadlocked
    bool deadlock = false;
    for (int i = 0; i < n; i++) {
        if (!Finish[i]) {
            deadlockedProcs[i] = true;
            deadlock = true;
        } else {
            deadlockedProcs[i] = false;
        }
    }
    return deadlock;
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