#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 10000

long sum = 0;
long solution[10000];
int solution_index = 0;

void solve(char *token);
bool invalid(char* value);

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    while (fgets(line, LINE_LEN, fp)) {
        // read each line
        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);

        while (token != NULL) {
            solve(token);
            token = strtok_r(NULL, ",", &saveptr);
            //printf("[DEBUG] Partial sum=%ld\n", sum);
        }
    }

    printf("[DEBUG] Solution Lenth=%d\n", solution_index);

    for (int i = 0; i < solution_index; i++) {
        printf("[DEBUG] Elem[%d]=%ld\n", i, solution[i]);
    }

    printf("[SOLUTION] The sum is %ld\n", sum);
}

void solve(char *token) {
    char *saveptr = NULL;
    char *start   = strtok_r(token, "-", &saveptr);
    char *end     = strtok_r(NULL, "\n", &saveptr);

    printf("[DEBUG] [%s-%s]\n", start, end);

    for (long i = atol(start); i <= atol(end); i++) {
        char buffer[20];
        sprintf(buffer, "%ld", i);
        if (invalid(buffer)) {
            //printf("[DEBUG] Invalid=%ld\n", i);
            sum += i;
            //printf("[DEBUG] Partial sum=%ld\n", sum);
        }
    }
}

bool invalid(char *value) {
    int size = 1;
    bool is_valid = true;

    while (size < strlen(value)) {
        char pattern[1000];
        is_valid = false;

        // copy the pattern
        for (int i = 0; i < size; i++) {
            pattern[i] = value[i];
        }
        pattern[size] = '\0';

        // generate the sequences
        for (int i = 0; i < strlen(value); i += size) {
            char current[1000];
            int k = 0;
            
            for (int j = i; j < i + size; j++, k++) {
                current[k] = value[j];
            }
            current[k] = '\0';

            if (strcmp(pattern, current) != 0) {
                is_valid = true;
                break;
            }
        }
        if (!is_valid) {
            solution[solution_index++] = atol(value);
            return true;
        }

        size++;
    }

    if (!is_valid) {
        solution[solution_index++] = atol(value);
    }
    return !is_valid;
}
