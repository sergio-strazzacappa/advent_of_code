#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 100
#define RANGES_MAX_SIZE 200
#define INGREDIENTS_MAX_SIZE 1500

typedef struct Range {
    long start;
    long end;
} Range;

int count = 0;

Range ranges[RANGES_MAX_SIZE];
int ranges_size = 0;

long ingredients[INGREDIENTS_MAX_SIZE];
int ingredients_size = 0;

void solve();

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    bool is_range = true;

    while (fgets(line, LINE_LEN, fp)) {
        // read each line
        
        if (line[0] == '\n') {
            is_range = false;
            continue;
        }

        if (is_range) {
            // read the ranges
            char *start = strtok(line, "-");
            char *end = strtok(NULL, "\n");

            Range range = { atol(start), atol(end) };
            ranges[ranges_size++] = range;
        } else {
            // read the available ingredients
            long ingredient = atol(line);

            ingredients[ingredients_size++] = ingredient;
        }
    }
    solve();
}

void solve() {
    for (int i = 0; i < ranges_size; i++) {
        printf("Range size [%ld - %ld]\n", ranges[i].start, ranges[i].end);
    }
    printf("\n");
    for (int i = 0; i < ingredients_size; i++) {
        printf("Ingredient=%ld\n", ingredients[i]);
    }

    for (int i = 0; i < ingredients_size; i++) {
        bool is_fresh = false;
        for (int j = 0; j < ranges_size; j++) {
            if (ingredients[i] >= ranges[j].start && ingredients[i] <= ranges[j].end) {
                is_fresh = true;
                break;
            }
        }
        if (is_fresh) {
            count++;
        }
    }
    printf("[SOLUTION] %d\n", count);
}
