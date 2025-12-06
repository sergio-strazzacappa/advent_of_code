#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 4000
#define MAX_OPERANDS 10
#define MAX_OPERATIONS 1000

int operands[MAX_OPERANDS][MAX_OPERATIONS];
int operands_size = 0;

char operators[MAX_OPERATIONS];

void solve();

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    for (int i = 0; i < MAX_OPERANDS; i++) {
        for (int j = 0; j < MAX_OPERATIONS; j++) {
            operands[i][j] = 0;
        }
    }

    for (int i = 0; i < MAX_OPERATIONS; i++) {
        operators[i] = ' ';
    }

    while (fgets(line, LINE_LEN, fp)) {
        // read each line

        char *token = strtok(line, " \n");
        int i = 0;

        while (token != NULL) {
            if (token[0] == '+' || token[0] == '*') {
                operators[i] = token[0];
            } else {
                operands[operands_size][i] = atoi(token);
            }

            i++;
            token = strtok(NULL, " \n");
        }
        operands_size++;
    }

    solve();
}

void solve() {
    long sum = 0;

    for (int i = 0; i < MAX_OPERATIONS; i++) {
        long partial_result = operands[0][i];
        for (int j = 1; j < MAX_OPERANDS; j++) {
            if (operands[j][i] != 0) {
                if (operators[i] == '+') {
                    partial_result += operands[j][i];
                } else if (operators[i] == '*') {
                    partial_result *= operands[j][i];
                } else {
                    printf("[ERROR] Incorrect operator\n");
                }
            }
        }
        sum += partial_result;
    }
    printf("[SOLUTION] %ld\n", sum);
}
