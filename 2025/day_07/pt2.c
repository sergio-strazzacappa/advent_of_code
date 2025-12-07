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
    long n;
} Beam;

char grid[ROWS][COLS];

void solve();
int search_beam(Beam beams[], int size, Beam b);
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

    solve();
}

void solve() {
    Beam beams[100000];
    int beams_size = 0;

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == 'S') {
                Beam b = { x, y + 1, 1};
                beams[beams_size++] = b;
            }
        }
    }

    long timelines = 1;

    while (true) {
        bool exit_loop = true;

        for (int i = 0; i < beams_size; i++) {
            if (beams[i].n > 0) {
                exit_loop = false;
                break;
            }
        }
       
        if (exit_loop) {
            break;
        }

        int new_size = beams_size;

        for (int i = 0; i < beams_size; i++) {
            if (beams[i].n == 0)
                continue;
            if (beams[i].y == ROWS - 1) {
                // the beam is at the bottom
                beams[i].n = 0;
                continue;
            }

            // move the beam down
            beams[i].y++;

            if (grid[beams[i].y][beams[i].x] == '^') {
                // split the beam
                timelines += beams[i].n;
                int x_left = beams[i].x - 1;
                int x_right = beams[i].x + 1;

                if (x_left >= 0) {
                    Beam b = {x_left, beams[i].y, beams[i].n};

                    int index = search_beam(beams, new_size, b);

                    if (index == -1) {
                        beams[new_size++] = b;
                    } else {
                        beams[index].n += beams[i].n;
                    }
                }

                if (x_right < COLS) {
                    Beam b = {x_right, beams[i].y, beams[i].n};

                    int index = search_beam(beams, new_size, b);

                    if (index == -1) {
                        beams[new_size++] = b;
                    } else {
                        beams[index].n += beams[i].n;
                    }
                }

                beams[i].n = 0;
            }
        }
        beams_size = new_size;

    }
    printf("[SOLUTION] %ld\n", timelines);
}

// return the index of the beam b if it in the beams array
// -1 if not found
int search_beam(Beam beams[], int size, Beam b) {
    for (int i = 0; i < size; i++) {
        if (beams[i].x == b.x && beams[i].y == b.y) {
            return i;
        }
    }
    return -1;
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
        printf("[DEBUG] (%d, %d) - %ld\n", beams[i].x, beams[i].y, beams[i].n);
    }
    printf("[DEBUG] ----------------------------------------\n");
}
