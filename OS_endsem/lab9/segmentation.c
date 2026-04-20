/**
 * Lab 9 Additional: Segmentation Simulation
 * Fully user-input driven
 * Compile: gcc segmentation.c -o segmentation
 * Run: ./segmentation
 */

#include <stdio.h>
#include <stdbool.h>

#define MAX_SEG 20

typedef struct {
    int base;
    int limit;
} SegmentEntry;

int main() {
    SegmentEntry seg_table[MAX_SEG];
    int num_segments;
    char choice;

    printf("\n========== SEGMENTATION ADDRESS TRANSLATION ==========\n");

    printf("Enter number of segments: ");
    scanf("%d", &num_segments);
    if (num_segments <= 0 || num_segments > MAX_SEG) {
        printf("Invalid number of segments. Exiting.\n");
        return 1;
    }

    printf("\nEnter base address and limit for each segment:\n");
    for (int i = 0; i < num_segments; i++) {
        printf("Segment %d - Base: ", i);
        scanf("%d", &seg_table[i].base);
        printf("Segment %d - Limit: ", i);
        scanf("%d", &seg_table[i].limit);
    }

    printf("\nSegment Table:\n");
    printf("Segment\tBase\tLimit\n");
    for (int i = 0; i < num_segments; i++) {
        printf("%d\t%d\t%d\n", i, seg_table[i].base, seg_table[i].limit);
    }

    do {
        int seg_num, offset;
        printf("\nEnter logical address (segment number and offset): ");
        scanf("%d %d", &seg_num, &offset);

        if (seg_num < 0 || seg_num >= num_segments) {
            printf("Error: Invalid segment number.\n");
        } else if (offset < 0 || offset >= seg_table[seg_num].limit) {
            printf("TRAP: Offset %d exceeds segment %d limit (%d).\n", 
                   offset, seg_num, seg_table[seg_num].limit);
        } else {
            int physical = seg_table[seg_num].base + offset;
            printf("Logical: (seg=%d, offset=%d) -> Physical Address: %d\n", 
                   seg_num, offset, physical);
        }

        printf("\nTranslate another address? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    // Demonstrate manual example
    printf("\n--- Manual Example (as in Figure 9.3) ---\n");
    printf("If you used segments with base/limit as in the manual:\n");
    printf("Seg2: base=4300, limit=400; Seg3: base=3200, limit=1100; Seg0: base=1400, limit=1000\n");
    printf("(seg=2, offset=53)  -> physical %d (if within limit)\n", 4300 + 53);
    printf("(seg=3, offset=852) -> physical %d\n", 3200 + 852);
    printf("(seg=0, offset=1222)-> trap (1222 > 1000)\n");

    return 0;
}