/**
 * Lab 9 Exercise 2: Paging Simulation
 * Fully user-input driven
 * Compile: gcc paging.c -o paging -lm
 * Run: ./paging
 */

#include <stdio.h>
#include <math.h>

int main() {
    int page_size, num_pages, total_memory;
    int logical_addr;
    char choice;

    printf("\n========== PAGING ADDRESS TRANSLATION ==========\n");

    printf("Enter page size (in bytes): ");
    scanf("%d", &page_size);
    if (page_size <= 0 || (page_size & (page_size - 1)) != 0) {
        printf("Warning: Page size should ideally be a power of 2.\n");
    }

    printf("Enter total number of pages in logical address space: ");
    scanf("%d", &num_pages);
    total_memory = page_size * num_pages;
    printf("Total logical address space: %d bytes (0 to %d)\n", total_memory, total_memory - 1);

    // Calculate bits required
    int bits_page = (int)ceil(log2(num_pages));
    int bits_offset = (int)ceil(log2(page_size));
    int bits_total = bits_page + bits_offset;
    printf("Address bits: %d total (%d for page number, %d for offset)\n\n", 
           bits_total, bits_page, bits_offset);

    do {
        printf("Enter logical address (0 to %d): ", total_memory - 1);
        scanf("%d", &logical_addr);

        if (logical_addr < 0 || logical_addr >= total_memory) {
            printf("Error: Address out of bounds.\n");
        } else {
            int page_num = logical_addr / page_size;
            int offset = logical_addr % page_size;
            printf("Logical Address: %d\n", logical_addr);
            printf("  Page Number: %d\n", page_num);
            printf("  Offset: %d\n", offset);
            printf("  Physical Address (assuming page i -> frame i): %d\n", 
                   page_num * page_size + offset);
        }

        printf("\nTranslate another address? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    // Also show the specific manual test cases
    printf("\n--- Manual Example with Current Parameters ---\n");
    printf("For addresses 204 and 56 (assuming 32-byte pages, 8 pages):\n");
    if (page_size == 32 && num_pages == 8) {
        printf("Address 204: Page = %d, Offset = %d\n", 204/32, 204%32);
        printf("Address 56: Page = %d, Offset = %d\n", 56/32, 56%32);
    } else {
        printf("Note: Manual example expects page_size=32 and num_pages=8.\n");
        printf("With your parameters:\n");
        printf("Address 204: Page = %d, Offset = %d\n", 204/page_size, 204%page_size);
        printf("Address 56: Page = %d, Offset = %d\n", 56/page_size, 56%page_size);
    }

    return 0;
}