#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 10000

long sum = 0;

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
            printf("[DEBUG] Partial sum=%ld\n", sum);
        }
    }
    printf("The sum is %ld\n", sum);
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
            printf("[DEBUG] Invalid=%ld\n", i);
            sum += i;
        }
    }
}

bool invalid(char *value) {
    if (strlen(value) % 2 == 1)
        return false;

    char first_half[1000], second_half[1000];
    strcpy(first_half, value);
    first_half[strlen(value) / 2] = '\0';
    strcpy(second_half, value + strlen(value) / 2);

    return (strcmp(first_half, second_half) == 0);
}
