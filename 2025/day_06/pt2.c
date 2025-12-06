#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 4000
#define MAX_OPERANDS 10000
#define MAX_OPERATIONS 10000
#define ROWS 10
#define COLS 4000

int digits[ROWS][COLS];
int operands[MAX_OPERANDS][MAX_OPERATIONS];
char operators[MAX_OPERATIONS];

void solve();

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            digits[y][x] = 0;
        }
    }

    int row = 0;
    while (fgets(line, LINE_LEN, fp)) {
        // read the operators
        if (line[0] == '+' || line[0] == '*') {
            char *token = strtok(line, " \n");
            int i = 0; 

            while (token != NULL) {
                operators[i] = token[0];
                token = strtok(NULL, " \n");
                i++;
            }
        } else {
            // create a matrix of digits
            for (int col = 0; col < strlen(line); col++) {
                if (line[col] == '\n') {
                    break;
                }

                if (line[col] == ' ') {
                    digits[row][col] = 0;
                } else {
                    digits[row][col] = line[col] - '0';
                }
            }
            row++;
        }
    }

    // build the operands
    int digit = 0;
    int op_row = 0;
    int op_col = 0;

    for (int x = 0; x < COLS; x++) {
        digit = 0;
        for (int y = 0; y < ROWS; y++) {
            if (digits[y][x] == 0) {
                continue;
            }
            digit = digit * 10 + digits[y][x];
        }
        if (digit != 0) {
            operands[op_row][op_col] = digit;
            op_row++;
        } else {
            op_row = 0;
            op_col++;
        }
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
