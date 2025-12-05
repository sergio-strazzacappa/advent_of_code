#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 150
#define ROWS 140
#define COLS 140

const int DIRS[8][2] = {
    { +0, -1 },
    { +1, -1 },
    { +1, +0 },
    { +1, +1 },
    { +0, +1 },
    { -1, +1 },
    { -1, +0 },
    { -1, -1 }
};

char map[140][140];
int rolls = 0;

void solve();
void print_map();

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    int y = 0;
    while (fgets(line, LINE_LEN, fp)) {
        // read each line

        for (int x = 0; x < COLS; x++) {
            map[y][x] = line[x];
        }

        y++;

    }

    solve();
    //print_map();
}

void solve() {
    int rolls_to_be_removed = 0;

    do {
        rolls += rolls_to_be_removed;
        rolls_to_be_removed = 0;

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                int adyacents = 0;

                if (map[y][x] == '@') {
                    for (int i = 0; i < 8; i++) {
                        int next_x = x + DIRS[i][0];
                        int next_y = y + DIRS[i][1];

                        if (next_x < 0)         continue;
                        if (next_x > COLS - 1)  continue;
                        if (next_y < 0)         continue;
                        if (next_y > ROWS - 1)  continue;

                        if (map[next_y][next_x] != '@' && map[next_y][next_x] != 'X') {
                            continue;
                        }

                        adyacents++;
                    }
                    if (adyacents < 4) {
                        rolls_to_be_removed++;
                        map[y][x] = 'X'; // mark for remove
                    }
                }
            }
        }

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                if (map[y][x] == 'X') {
                    map[y][x] = '.';
                }
            }
        }
    } while (rolls_to_be_removed);

    printf("[SOLUTION] %d\n", rolls);
}

void print_map() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}
