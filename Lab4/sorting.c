#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int *arr;
int n;

void* bubble_sort(void* arg) {
    clock_t start = clock();
    int *temp_arr = malloc(n * sizeof(int));
    for(int i=0; i<n; i++) temp_arr[i] = arr[i];

    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (temp_arr[j] > temp_arr[j+1]) {
                int temp = temp_arr[j];
                temp_arr[j] = temp_arr[j+1];
                temp_arr[j+1] = temp;
            }
        }
    }
    
    double time_taken = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Bubble Sort completed in %f seconds.\n", time_taken);
    
    free(temp_arr);
    pthread_exit(0);
}

void* selection_sort(void* arg) {
    clock_t start = clock();
    
    int *temp_arr = malloc(n * sizeof(int));
    for(int i=0; i<n; i++) temp_arr[i] = arr[i];

    for (int i = 0; i < n-1; i++) {
        int min = i;
        for (int j = i+1; j < n; j++) {
            if (temp_arr[j] < temp_arr[min]) min = j;
        }
        int temp = temp_arr[min];
        temp_arr[min] = temp_arr[i];
        temp_arr[i] = temp;
    }

    double time_taken = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Selection Sort completed in %f seconds.\n", time_taken);

    for(int i=0; i<n; i++) arr[i] = temp_arr[i]; 
    
    free(temp_arr);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No numbers provided.\n");
        printf("Usage: %s <num1> <num2> <num3> ...\n", argv[0]);
        printf("Example: ./sorting 10 5 2 7\n");
        return 1;
    }

    n = argc - 1;
    arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i+1]);
    }

    printf("Main thread: Sorting %d elements...\n", n);

    pthread_t t1, t2;

    if(pthread_create(&t1, NULL, bubble_sort, NULL) != 0) {
        perror("Failed to create Bubble Sort thread");
    }
    if(pthread_create(&t2, NULL, selection_sort, NULL) != 0) {
        perror("Failed to create Selection Sort thread");
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final Sorted Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}