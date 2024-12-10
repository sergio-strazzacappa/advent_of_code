#include <stdio.h>
#include <string.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define COL         1000

void dump(char table[COL][LINE_LEN], int n, int m);
int part1(char table[COL][LINE_LEN], int n, int m);
int part2(char table[COL][LINE_LEN], int n, int m);

int main() {
    FILE *fp;
    char line[1000];
    int n, m, answer1, answer2;

    char table[COL][LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }
    
    n = 0;

    while (fgets(line, LINE_LEN, fp)) {
        for (m = 0; m < strlen(line); m++)
            if (line[m] != '\n')
                table[n][m] = line[m];
        n++;
    }

    //dump(table, n, m);

    answer1 = part1(table, n, m);
    answer2 = part2(table, n, m);

    printf("The answer to part 1 is %d\n", answer1);
    printf("The answer to part 2 is %d\n", answer2);

    fclose(fp);

    return 0;
}

void dump(char table[COL][LINE_LEN], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%c", table[i][j]);
        }
        printf("\n");
    }
}

int part1(char table[COL][LINE_LEN], int n, int m) {
    int answer = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (table[i][j] == 'X') {
                if (j + 3 < m && 
                    table[i][j + 1] == 'M' &&
                    table[i][j + 2] == 'A' && 
                    table[i][j + 3] == 'S') {
                    // right
                    //printf("Find in [%d, %d] to the right\n", i, j);
                    answer++;
                }
                if (j + 3 < m && i + 3 < n &&
                    table[i + 1][j + 1] == 'M' &&
                    table[i + 2][j + 2] == 'A' && 
                    table[i + 3][j + 3] == 'S') {
                    // right/down
                    //printf("Find in [%d, %d] to the right/down\n", i, j);
                    answer++;
                }
                if (i + 3 < n && 
                    table[i + 1][j] == 'M' &&
                    table[i + 2][j] == 'A' && 
                    table[i + 3][j] == 'S') {
                    // down
                    //printf("Find in [%d, %d] to the down\n", i, j);
                    answer++;
                }
                if (j - 3 > -1 && i + 3 < n && 
                    table[i + 1][j - 1] == 'M' &&
                    table[i + 2][j - 2] == 'A' &&
                    table[i + 3][j - 3] == 'S') {
                    // left/down
                    //printf("Find in [%d, %d] to the left/down\n", i, j);
                    answer++;
                }
                if (j - 3 > -1 && 
                    table[i][j - 1] == 'M' &&
                    table[i][j - 2] == 'A' &&
                    table[i][j - 3] == 'S') {
                    // left
                    //printf("Find in [%d, %d] to the left\n", i, j);
                    answer++;
                }
                if (j - 3 > -1 && i - 3 > -1 && 
                    table[i - 1][j - 1] == 'M' &&
                    table[i - 2][j - 2] == 'A' &&
                    table[i - 3][j -  3] == 'S') {
                    // left/up
                    //printf("Find in [%d, %d] to the left/up\n", i, j);
                    answer++;
                }
                if (i - 3 > -1 && 
                    table[i - 1][j] == 'M' &&
                    table[i - 2][j] == 'A' &&
                    table[i - 3][j] == 'S') {
                    // up
                    //printf("Find in [%d, %d] to the up\n", i, j);
                    answer++;
                }
                if (j + 3 < m && i - 3 > - 1 &&
                    table[i - 1][j + 1] == 'M' &&
                    table[i - 2][j + 2] == 'A' &&
                    table[i - 3][j + 3] == 'S') {
                    // right/up
                    //printf("Find in [%d, %d] to the right/up\n", i, j);
                    answer++;
                }
            }
        }
    }
    return answer;
}

int part2(char table[COL][LINE_LEN], int n, int m) {
    int answer = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // out of bonds
            if (!(j > 0 && j < m - 1) || !(i > 0 && i < n - 1)) continue;

            if (table[i][j] == 'A') {
                if (table[i - 1][j - 1] == 'M' &&
                    table[i + 1][j + 1] == 'S' &&
                    table[i - 1][j + 1] == 'M' &&
                    table[i + 1][j - 1] == 'S') {
                    //printf("Find in [%d, %d]\n", i, j);
                    answer++;
                }
                if (table[i - 1][j - 1] == 'S' &&
                    table[i + 1][j + 1] == 'M' &&
                    table[i - 1][j + 1] == 'M' &&
                    table[i + 1][j - 1] == 'S') {
                    //printf("Find in [%d, %d]\n", i, j);
                    answer++;
                }
                if (table[i - 1][j - 1] == 'M' &&
                    table[i + 1][j + 1] == 'S' &&
                    table[i - 1][j + 1] == 'S' &&
                    table[i + 1][j - 1] == 'M') {
                    //printf("Find in [%d, %d]\n", i, j);
                    answer++;
                }
                if (table[i - 1][j - 1] == 'S' &&
                    table[i + 1][j + 1] == 'M' &&
                    table[i - 1][j + 1] == 'S' &&
                    table[i + 1][j - 1] == 'M') {
                    //printf("Find in [%d, %d]\n", i, j);
                    answer++;
                }
            }
        }
    }
    return answer;
}
