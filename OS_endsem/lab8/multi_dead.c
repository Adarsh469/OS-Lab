/**
 * Multithreaded Deadlock Detection Simulation
 * Lab 8 - Additional Concept (Detection with Threads)
 * 
 * Compile: gcc detection_threaded.c -o detection_threaded -lpthread
 * Run: ./detection_threaded
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
int Allocation[MAX_P][MAX_R];           // currently allocated resources
int Request[MAX_P][MAX_R];              // outstanding requests (what each process is waiting for)
int Available[MAX_R];                   // available instances of each resource

// Synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[MAX_P];             // condition variable per process (for waiting)

bool deadlock_detected = false;         // flag set by detector thread

// Function prototypes
void printState();
bool detectDeadlock(bool deadlockedProcs[]);
void requestResources(int pid, int req[]);
void releaseResources(int pid, int rel[]);

// Detector thread: runs periodically to check for deadlock
void* detectorThread(void* arg) {
    int interval = *(int*)arg;
    free(arg);
    
    while (!deadlock_detected) {
        sleep(interval);
        
        pthread_mutex_lock(&mutex);
        bool deadlocked[MAX_P] = {false};
        if (detectDeadlock(deadlocked)) {
            deadlock_detected = true;
            printf("\n========== DEADLOCK DETECTED ==========\n");
            printf("Deadlocked processes: ");
            for (int i = 0; i < n; i++) {
                if (deadlocked[i]) printf("P%d ", i);
            }
            printf("\n");
            printf("=======================================\n\n");
            pthread_mutex_unlock(&mutex);
            break;
        } else {
            printf("[Detector] No deadlock found.\n");
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Process thread: simulates a process making requests and releases
void* processThread(void* arg) {
    int pid = *(int*)arg;
    free(arg);
    
    srand(time(NULL) ^ (pid * 100));
    
    while (!deadlock_detected) {
        // Decide randomly: request or release
        int action = rand() % 2;
        
        pthread_mutex_lock(&mutex);
        
        if (action == 0) {   // Request resources
            int req[MAX_R];
            for (int j = 0; j < m; j++) {
                req[j] = rand() % 3;   // request 0, 1, or 2 of each resource type
            }
            
            printf("[P%d] Requests: ", pid);
            for (int j = 0; j < m; j++) printf("%d ", req[j]);
            printf("\n");
            
            requestResources(pid, req);
            
        } else {             // Release resources
            int rel[MAX_R];
            for (int j = 0; j < m; j++) {
                if (Allocation[pid][j] > 0)
                    rel[j] = rand() % (Allocation[pid][j] + 1);
                else
                    rel[j] = 0;
            }
            
            if (rel[0] > 0 || rel[1] > 0 || rel[2] > 0) {   // only if actually releasing something
                printf("[P%d] Releases: ", pid);
                for (int j = 0; j < m; j++) printf("%d ", rel[j]);
                printf("\n");
                releaseResources(pid, rel);
            }
        }
        
        printState();
        pthread_mutex_unlock(&mutex);
        
        sleep(1 + rand() % 2);   // wait before next action
    }
    return NULL;
}

int main() {
    printf("\n========== MULTITHREADED DEADLOCK DETECTION ==========\n\n");
    
    // Input number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    
    // Initialize condition variables
    for (int i = 0; i < n; i++) {
        pthread_cond_init(&cond[i], NULL);
    }
    
    // Input initial Allocation matrix
    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }
    
    // Input initial Request matrix
    printf("\nEnter Request Matrix (%d x %d) [current outstanding requests]:\n", n, m);
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
    
    printf("\nInitial state:\n");
    printState();
    
    // Initial deadlock check
    bool initialDeadlocked[MAX_P] = {false};
    if (detectDeadlock(initialDeadlocked)) {
        printf("Initial state is DEADLOCKED!\n");
        return 1;
    } else {
        printf("Initial state is DEADLOCK-FREE.\n");
    }
    
    // Create detector thread
    pthread_t detector;
    int* interval = malloc(sizeof(int));
    *interval = 3;   // check every 3 seconds
    pthread_create(&detector, NULL, detectorThread, interval);
    
    // Create n process threads
    pthread_t procs[MAX_P];
    for (int i = 0; i < n; i++) {
        int* pid = malloc(sizeof(int));
        *pid = i;
        pthread_create(&procs[i], NULL, processThread, pid);
    }
    
    // Wait for threads (they will exit when deadlock detected)
    for (int i = 0; i < n; i++) {
        pthread_join(procs[i], NULL);
    }
    pthread_join(detector, NULL);
    
    // Cleanup
    for (int i = 0; i < n; i++) {
        pthread_cond_destroy(&cond[i]);
    }
    pthread_mutex_destroy(&mutex);
    
    printf("\nSimulation ended.\n");
    return 0;
}

// Deadlock detection algorithm (as per Lab 8)
bool detectDeadlock(bool deadlockedProcs[]) {
    int Work[MAX_R];
    bool Finish[MAX_P];
    
    // Step 1: Initialize Work = Available
    for (int j = 0; j < m; j++) {
        Work[j] = Available[j];
    }
    
    // Step 2: Finish[i] = true if Allocation[i] == 0
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
    
    // Step 3: Find an i such that !Finish[i] and Request[i] <= Work
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
                }
            }
        }
    } while (changed);
    
    // Step 4: Any process with Finish[i] == false is deadlocked
    bool deadlock = false;
    for (int i = 0; i < n; i++) {
        deadlockedProcs[i] = !Finish[i];
        if (!Finish[i]) deadlock = true;
    }
    return deadlock;
}

void requestResources(int pid, int req[]) {
    // Check if resources are available
    bool available = true;
    for (int j = 0; j < m; j++) {
        if (req[j] > Available[j]) {
            available = false;
            break;
        }
    }
    
    if (available) {
        // Grant immediately
        for (int j = 0; j < m; j++) {
            Available[j] -= req[j];
            Allocation[pid][j] += req[j];
        }
        printf("[P%d] Request GRANTED immediately.\n", pid);
    } else {
        // Resources not available; add to Request (wait)
        for (int j = 0; j < m; j++) {
            Request[pid][j] += req[j];
        }
        printf("[P%d] Request QUEUED (insufficient resources).\n", pid);
    }
}

void releaseResources(int pid, int rel[]) {
    for (int j = 0; j < m; j++) {
        if (rel[j] > Allocation[pid][j]) {
            rel[j] = Allocation[pid][j];   // sanity check
        }
        Available[j] += rel[j];
        Allocation[pid][j] -= rel[j];
    }
    
    // After release, check if any waiting requests can now be satisfied
    // (Simple version: we could iterate all processes and try to fulfill their Request)
    for (int i = 0; i < n; i++) {
        if (i == pid) continue;
        bool canFulfill = true;
        for (int j = 0; j < m; j++) {
            if (Request[i][j] > Available[j]) {
                canFulfill = false;
                break;
            }
        }
        if (canFulfill && (Request[i][0] > 0 || Request[i][1] > 0 || Request[i][2] > 0)) {
            // Grant the queued request
            for (int j = 0; j < m; j++) {
                Available[j] -= Request[i][j];
                Allocation[i][j] += Request[i][j];
                Request[i][j] = 0;
            }
            printf("[P%d] Queued request GRANTED after release.\n", i);
        }
    }
}

void printState() {
    printf("\n--- Current State ---\n");
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", Available[j]);
    printf("\nAllocation:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) printf("%d ", Allocation[i][j]);
        printf("\n");
    }
    printf("Request:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) printf("%d ", Request[i][j]);
        printf("\n");
    }
    printf("---------------------\n");
}