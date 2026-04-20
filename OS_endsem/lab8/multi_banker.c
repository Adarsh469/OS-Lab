/**
 * Multithreaded Banker's Algorithm
 * Lab 8 - Additional Exercise 1
 * 
 * Compile: gcc bankers_threaded.c -o bankers_threaded -lpthread
 * Run: ./bankers_threaded
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_P 20
#define MAX_R 20

// Global shared data structures
int n, m;                               // number of processes, resource types
int Max[MAX_P][MAX_R];                  // maximum demand of each process
int Allocation[MAX_P][MAX_R];           // currently allocated resources
int Need[MAX_P][MAX_R];                 // remaining need = Max - Allocation
int Available[MAX_R];                   // available instances of each resource

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // protects all shared data

// Function prototypes
void computeNeed();
bool isSafe(int *safeSeq);
bool requestResources(int pid, int req[]);
void releaseResources(int pid, int rel[]);
void printState();

// Thread function: simulates a process making requests and releases
void* processThread(void* arg) {
    int pid = *(int*)arg;
    free(arg);
    
    srand(time(NULL) ^ (pid << 4));   // seed for random requests
    
    for (int iterations = 0; iterations < 5; iterations++) {
        // Generate a random request vector (each entry between 0 and Need[pid][j])
        int req[MAX_R];
        int rel[MAX_R];
        
        pthread_mutex_lock(&mutex);
        for (int j = 0; j < m; j++) {
            if (Need[pid][j] > 0)
                req[j] = rand() % (Need[pid][j] + 1);
            else
                req[j] = 0;
        }
        
        printf("\n[P%d] Requests: ", pid);
        for (int j = 0; j < m; j++) printf("%d ", req[j]);
        printf("\n");
        
        bool granted = requestResources(pid, req);
        if (granted) {
            printf("[P%d] Request GRANTED.\n", pid);
            printState();
            pthread_mutex_unlock(&mutex);
            
            // Simulate work
            sleep(1 + rand() % 3);
            
            // Now release some resources (up to currently allocated)
            pthread_mutex_lock(&mutex);
            for (int j = 0; j < m; j++) {
                if (Allocation[pid][j] > 0)
                    rel[j] = rand() % (Allocation[pid][j] + 1);
                else
                    rel[j] = 0;
            }
            printf("[P%d] Releases: ", pid);
            for (int j = 0; j < m; j++) printf("%d ", rel[j]);
            printf("\n");
            releaseResources(pid, rel);
            printState();
        } else {
            printf("[P%d] Request DENIED (unsafe or exceeds claim).\n", pid);
        }
        pthread_mutex_unlock(&mutex);
        
        sleep(1);   // wait before next attempt
    }
    
    return NULL;
}

int main() {
    printf("\n========== MULTITHREADED BANKER'S ALGORITHM ==========\n\n");
    
    // Input number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    
    // Input Max matrix
    printf("\nEnter Max Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Max[i][j]);
        }
    }
    
    // Input Available vector
    printf("\nEnter initial Available Resources (vector of size %d):\n", m);
    for (int j = 0; j < m; j++) {
        scanf("%d", &Available[j]);
    }
    
    // Initialize Allocation to zero, Need = Max
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Allocation[i][j] = 0;
            Need[i][j] = Max[i][j];
        }
    }
    
    printf("\nInitial state:\n");
    printState();
    
    // Initial safety check
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        printf("Initial state is SAFE. Sequence: ");
        for (int i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
        printf("\n\n");
    } else {
        printf("Initial state is UNSAFE. Exiting.\n");
        return 1;
    }
    
    // Create n threads (one per process)
    pthread_t threads[MAX_P];
    for (int i = 0; i < n; i++) {
        int* pid = malloc(sizeof(int));
        *pid = i;
        if (pthread_create(&threads[i], NULL, processThread, pid) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nAll processes completed. Final state:\n");
    printState();
    
    pthread_mutex_destroy(&mutex);
    return 0;
}

void computeNeed() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}

bool isSafe(int *safeSeq) {
    int Work[MAX_R];
    bool Finish[MAX_P] = {false};
    
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
            return false;
        }
    }
    return true;
}

bool requestResources(int pid, int req[]) {
    // Check Request <= Need
    for (int j = 0; j < m; j++) {
        if (req[j] > Need[pid][j]) {
            return false;
        }
    }
    // Check Request <= Available
    for (int j = 0; j < m; j++) {
        if (req[j] > Available[j]) {
            return false;
        }
    }
    
    // Tentatively allocate
    for (int j = 0; j < m; j++) {
        Available[j] -= req[j];
        Allocation[pid][j] += req[j];
        Need[pid][j] -= req[j];
    }
    
    // Check safety
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        return true;   // keep changes
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

void releaseResources(int pid, int rel[]) {
    // Check release <= Allocation
    for (int j = 0; j < m; j++) {
        if (rel[j] > Allocation[pid][j]) {
            rel[j] = Allocation[pid][j];   // release only what is held
        }
    }
    for (int j = 0; j < m; j++) {
        Available[j] += rel[j];
        Allocation[pid][j] -= rel[j];
        Need[pid][j] += rel[j];   // Need increases when resources are released
    }
}

void printState() {
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", Available[j]);
    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) printf("%d ", Allocation[i][j]);
        printf("\n");
    }
    printf("Need Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) printf("%d ", Need[i][j]);
        printf("\n");
    }
}