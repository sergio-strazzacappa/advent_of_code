#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE          "input.txt"
#define LINE_LEN            3000
#define NUMBER_PATTERNS     3000
#define NUMBER_DESIGNS      500
#define CACHE_SIZE          1000

typedef struct map {
    char *design;
    long n;
} Map;

int part1(char *design, char *patterns[], int patterns_index, Map cache[],
        int *cache_index);
long part2(char *design, char *patterns[], int patterns_index, Map cache[],
        int *cache_index);
void insert_cache(char *s, long val, Map cache[], int *cache_index);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    char *token;
    char *patterns[NUMBER_PATTERNS], *designs[NUMBER_DESIGNS];
    int line_n, patterns_index, designs_index, cache_index;
    int possibilities;
    long sum;
    Map cache[CACHE_SIZE];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    line_n = 1;
    patterns_index = 0;
    designs_index = 0;

    while (fgets(line, LINE_LEN, fp)) {
        if (line_n == 1) {
            token = strtok(line, ", \n");

            while (token != NULL) {
                patterns[patterns_index] = (char *)malloc(sizeof(char) * 100);
                strcpy(patterns[patterns_index++], token);
                token = strtok(NULL, ", \n");
            }
        }
        if (line_n >= 3) {
            token = strtok(line, "\n");
            designs[designs_index] = (char *)malloc(sizeof(char) * 1000);
            strcpy(designs[designs_index++], token);
        }
        line_n++;
    }

    possibilities = 0;
    sum = 0;
    for (int i = 0; i < designs_index; i++) {
        cache_index = 0;
        possibilities +=
            part1(designs[i], patterns, patterns_index, cache, &cache_index);
        cache_index = 0;
        sum +=
            part2(designs[i], patterns, patterns_index, cache, &cache_index);
    }

    printf("Possibilities: %d\n", possibilities);
    printf("Sum: %ld\n", sum);

    for (int i = 0; i < patterns_index; i++) {
        free(patterns[i]);
    }
    for (int i = 0; i < designs_index; i++) {
        free(designs[i]);
    }

    for (int i = 0; i < cache_index; i++) {
        free(cache[i].design);
    }

    fclose(fp);
    return 0;
}

int part1(char *design, char *patterns[], int patterns_index, Map cache[],
        int *cache_index) {
    int success, sum;

    sum = 0;
    if (strlen(design) == 0) {
        return 1;
    }


    for (int i = 0; i < *cache_index; i++) {
        if (strcmp(design, cache[i].design) == 0) {
            return cache[i].n;
        }
    }

    for (int i = 0; i < patterns_index; i++) {
        if (strlen(design) < strlen(patterns[i])) {
            continue;
        }

        success = 1;
        for (int j = 0; j < strlen(patterns[i]); j++) {
            if (design[j] != patterns[i][j]) {
                success = 0;
                break;
            }
        }

        if (success) {
            char substr[strlen(design)];
            int k, l;
            l = 0;
            for (k = strlen(patterns[i]); k < strlen(design); k++) {
                substr[l++] = design[k];
            }
            substr[l] = '\0';

            sum = part1(substr, patterns, patterns_index, cache, cache_index);
            insert_cache(substr, sum, cache, cache_index);

            if (sum)
                return sum;
        }
    }
    return sum;
}

long part2(char *design, char *patterns[], int patterns_index, Map cache[],
        int *cache_index) {
    int success, len;
    long count;
    char substr[strlen(design)];
 
    if (strlen(design) == 0) {
        // solution found
        return 1;
    }

    for (int i = 0; i < *cache_index; i++) {
        if (strcmp(design, cache[i].design) == 0) {
            // get store result
            return cache[i].n;
        }
    }

    // get the lenth of min(design, max(patterns))
    len = 0;
    for (int i = 0; i < patterns_index; i++) {
        if (strlen(patterns[i]) > len) {
            len = strlen(patterns[i]);
        }
    }

    if (strlen(design) < len) {
        len = strlen(design);
    }

    count = 0;
    for (int i = 0; i < len; i++) {
        success = 0;
        for (int j = 0; j < patterns_index; j++) {
            // check if some pattern matches the beginning of design
            if (i != strlen(patterns[j]) - 1)
                continue;
            int k;
            for (k = 0; k <= i; k++) {
                if (design[k] != patterns[j][k]) {
                    // not matched
                    break;
                }
            }
            if (k == strlen(patterns[j])) {
                // full pattern matches
                success = 1;
            }
            if (success) {
                // copy design without current patterns for next iteration
                int k, l;
                l = 0;
                for (k = strlen(patterns[j]); k < strlen(design); k++) {
                    substr[l++] = design[k];
                }
                substr[l] = '\0';
                break;
            }
        }
        if (success) {
            count += 
                part2(substr, patterns, patterns_index, cache, cache_index);
            insert_cache(substr, count, cache, cache_index);
        }
    }

    insert_cache(design, count, cache, cache_index);
    return count;
}

void insert_cache(char *s, long val, Map map[], int *map_index) {
    Map m;
    m.design = (char *)malloc(sizeof(char) * NUMBER_PATTERNS);
    m.n = val;
    strcpy(m.design, s);
    map[(*map_index)++] = m;
}
