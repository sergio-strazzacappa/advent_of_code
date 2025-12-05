#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 100
#define RANGES_MAX_SIZE 200
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

typedef struct Range {
    long start;
    long end;
} Range;

long count = 0;

Range ranges[RANGES_MAX_SIZE];
int ranges_size = 0;

void solve();
int cmp(const void *a, const void *b);

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    while (fgets(line, LINE_LEN, fp)) {
        // read each line
        
        if (line[0] == '\n') {
            break;
        }

        // read the ranges
        char *start = strtok(line, "-");
        char *end = strtok(NULL, "\n");

        Range range = { atol(start), atol(end) };
        ranges[ranges_size++] = range;
    }
    solve();
}

void solve() {
    qsort(ranges, ranges_size, sizeof(Range), cmp);

    for (int i = 0; i < ranges_size; i++) {
        printf("Range size [%ld - %ld]\n", ranges[i].start, ranges[i].end);
    }

    long end = -1;
    for (int i = 0; i < ranges_size; i++) {
        long start = MAX(ranges[i].start, end + 1);

        if (ranges[i].end >= start) {
            printf("start=%ld, end=%ld, ranges=%ld\n", start, ranges[i].end, ranges[i].end - start + 1);
            count += ranges[i].end - start + 1;
            end = ranges[i].end;
        }
    }

    printf("[SOLUTION] %ld\n", count);
}

int cmp(const void *a, const void *b) {
    const Range *range1 = (const Range *)a;
    const Range *range2 = (const Range *)b;

    if (range1->start < range2->start) {
        return -1;
    } else if (range1->start == range2->start) {
        if (range1->end < range2->end) {
            return -1;
        } else if (range1->end == range2->end) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}
