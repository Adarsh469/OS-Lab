#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    int studentIDs[n], pages[n];

    printf("\nEnter StudentID and Pages for each student:\n");
    for (int i = 0; i < n; i++) {
        printf("Student %d - ID: ", i + 1);
        scanf("%d", &studentIDs[i]);
        printf("Student %d - Pages: ", i + 1);
        scanf("%d", &pages[i]);
    }

    printf("\n--- Student Printing Details ---\n");
    printf("%-15s %-10s\n", "StudentID", "Pages");
    printf("-------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-15d %-10d\n", studentIDs[i], pages[i]);
    }
    printf("-------------------------\n");

    int total_args = 2 + 2 * n;
    char *args[total_args + 1];

    args[0] = "./printerAccess";

    char n_str[16];
    sprintf(n_str, "%d", n);
    args[1] = n_str;

    char id_strs[n][16], page_strs[n][16];
    for (int i = 0; i < n; i++) {
        sprintf(id_strs[i], "%d", studentIDs[i]);
        sprintf(page_strs[i], "%d", pages[i]);
        args[2 + 2 * i]     = id_strs[i];
        args[3 + 2 * i] = page_strs[i];
    }
    args[total_args] = NULL;

    printf("\nExecuting printerAccess program...\n\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        execv("./printerAccess", args);
        perror("execv failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("\nParent: printerAccess execution completed.\n");
    }

    return 0;
}