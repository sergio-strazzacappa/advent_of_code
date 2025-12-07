#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 150
#define ROWS 142 // 16 - 142
#define COLS 141 // 15 - 141

typedef struct Beam {
    int x;
    int y;
    bool active;
} Beam;

char grid[ROWS][COLS];

void solve();
bool search_beam(Beam beams[], int size, Beam b);
void print_grid(char grid[ROWS][COLS]);
void print_beams(Beam beams[], int size);

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
        for (int x = 0; x < strlen(line) - 1; x++) {
            grid[y][x] = line[x];
        }
        y++;
    }

    //print_grid(grid);
    solve();
}

void solve() {
    Beam beams[10000];
    int beams_size = 0;

    char copy[ROWS][COLS];

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            copy[y][x] = grid[y][x];
        }
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == 'S') {
                Beam b = { x, y + 1, true};
                beams[beams_size++] = b;
            }
        }
    }

    int splits = 0;

    while (true) {
        for (int i = 0; i < beams_size; i++) {
            if (beams[i].active) {
                copy[beams[i].y][beams[i].x] = '|';
            }
        }

        bool exit_loop = true;

        for (int i = 0; i < beams_size; i++) {
            if (beams[i].active) {
                exit_loop = false;
                break;
            }
        }
       
        if (exit_loop) {
            break;
        }

        int new_size = beams_size;

        for (int i = 0; i < beams_size; i++) {
            if (!beams[i].active)
                // the beam is inactive
                continue;
            if (beams[i].y == ROWS - 1) {
                // the beam is at the bottom
                beams[i].active = false;
                continue;
            }
            // move the beam down
            beams[i].y++;

            if (search_beam(beams, new_size, beams[i])) {
                for (int j = 0; j < new_size; j++) {
                    if (i == j)
                        continue;
                    if (beams[i].x == beams[j].x && beams[i].y == beams[j].y) {
                        beams[i].active = false;
                        break;
                    }
                }
            }

            if (grid[beams[i].y][beams[i].x] == '^') {
                // split the beam
                beams[i].active = false;
                int x_left = beams[i].x - 1;
                int x_right = beams[i].x + 1;
                splits++;

                if (x_left >= 0) {
                    Beam b = {x_left, beams[i].y, true};

                    if (!search_beam(beams, new_size, b)) {
                        beams[new_size++] = b;
                    }
                }
                if (x_right < COLS) {
                    Beam b = {x_right, beams[i].y, true};

                    if (!search_beam(beams, new_size, b)) {
                        beams[new_size++] = b;
                    }
                }
            }
        }
        beams_size = new_size;

    }
    printf("[SOLUTION] %d\n", splits);
}

// return true of the beam b is already in the beams array
bool search_beam(Beam beams[], int size, Beam b) {
    for (int i = 0; i < size; i++) {
        if (beams[i].x == b.x && beams[i].y == b.y) {
            return true;
        }
    }
    return false;
}

void print_grid(char grid[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }
}

void print_beams(Beam beams[], int size) {
    printf("[DEBUG] ----------------------------------------\n");
    for (int i = 0; i < size; i++) {
        printf("[DEBUG] (%d, %d) - %s\n", beams[i].x, beams[i].y,
            beams[i].active ? "active" : "inactive");
    }
    printf("[DEBUG] ----------------------------------------\n");
}
