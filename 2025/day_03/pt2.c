#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 150
#define BANK_OUTPUTS 12

long sum = 0;

int get_max(int bank[], int bank_size, int* start, int limit);

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    while (fgets(line, LINE_LEN, fp)) {
        // read each line
        
        int bank[LINE_LEN];
        int bank_size = 0;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\n') {
                break;
            }
            bank[bank_size++] = line[i] - '0';
        }

        int start = 0;
        long value = 0;
        for (int i = 0; i < BANK_OUTPUTS; i++) {
            int max_value = get_max(bank, bank_size, &start, bank_size - (BANK_OUTPUTS - 1) + i);
            value = value * 10 + max_value;
        }

        sum += value;

    }

    printf("[SOLUTION] %ld\n", sum);
}

int get_max(int bank[], int bank_size, int* start, int limit) {
    //printf("[DEBUG] bank_size=%d, start=%d, limit=%d)\n", bank_size, *start, limit);

    int max_value = 0;
    int max_index = 0;

    for (int i = (*start); i < bank_size; i++) {
        if (max_value == 9 || i == limit) {
            break;
        }

        if (bank[i] > max_value) {
            max_value = bank[i];
            max_index = i;
        }
    }

    bank[max_index] = 0;
    *start = max_index + 1;

    return max_value;
}
