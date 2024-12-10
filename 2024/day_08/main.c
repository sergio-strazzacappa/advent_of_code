#include <stdio.h>

#define INPUT_FILE  "input.txt"
#define W           1000
#define H           1000
#define Q_ANTENNA   1000
#define Q_ANTINODE  8000 

typedef struct antenna {
    int r;
    int c;
    char type;
} antenna;

void part1(char map[H][W], int rows, int cols, antenna antennas[Q_ANTENNA],
        int antenna_index);
void part2(char map[H][W], int rows, int cols, antenna antennas[Q_ANTENNA],
        int antenna_index);
void print_map(char map[H][W], int rows, int cols);
void print_antennas(antenna antennas[Q_ANTENNA], int antenna_index);
void print_antinodes(antenna antinodes[Q_ANTINODE], int antinode_index);

void draw_antinodes(char map[H][W], int rows, int cols,
        antenna antinodes[Q_ANTINODE], int antinode_index);

int main() {
    FILE *fp;
    char line[1000], map[H][W];
    int rows, cols, antenna_index;
    antenna antennas[Q_ANTENNA];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("Can't open the file %s\n", INPUT_FILE);
        return 1;
    }

    rows = 0;
    while (fgets(line, 1000, fp)) {
        cols = 0;
        for (int i = 0; line[i] != '\n'; i++) {
            map[rows][cols] = line[i];
            cols++;
        }
        rows++;
    }

    antenna_index = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] != '.') {
                // it's an antenna
                antenna a = { r, c, map[r][c] };
                antennas[antenna_index] = a; 
                antenna_index++;
            }
        }
    }

    part1(map, rows, cols, antennas, antenna_index);
    part2(map, rows, cols, antennas, antenna_index);

    fclose(fp);

    return 0;
}

void part1(char map[H][W], int rows, int cols, antenna antennas[Q_ANTENNA], int antenna_index) {
    antenna antinodes[Q_ANTINODE];
    int antinode_index, cant;
    
    antinode_index = 0;
    for (int i = 0; i < antenna_index; i++) {
        for (int j = i + 1; j < antenna_index; j++) {
            if (antennas[i].type == antennas[j].type) {
                // same type of antenna
                antenna a, b;
                if (antennas[i].r < antennas[j].r && antennas[i].c < antennas[j].c) {
                    a.r = antennas[i].r - (antennas[j].r - antennas[i].r); 
                    a.c = antennas[i].c - (antennas[j].c - antennas[i].c); 
                    a.type = antennas[i].type;

                    b.r = antennas[j].r + (antennas[j].r - antennas[i].r); 
                    b.c = antennas[j].c + (antennas[j].c - antennas[i].c);
                    b.type = antennas[i].type;
                } else if (antennas[i].r < antennas[j].r && antennas[i].c > antennas[j].c) {
                    a.r = antennas[i].r - (antennas[j].r - antennas[i].r); 
                    a.c = antennas[i].c + (antennas[i].c - antennas[j].c); 
                    a.type = antennas[i].type;

                    b.r = antennas[j].r + (antennas[j].r - antennas[i].r); 
                    b.c = antennas[j].c - (antennas[i].c - antennas[j].c);
                    b.type = antennas[i].type;
                } else if (antennas[i].r == antennas[j].r && antennas[i].c < antennas[j].c) {
                    // horizontal
                    a.r = antennas[i].r; 
                    a.c = antennas[i].c - (antennas[j].c - antennas[i].c); 
                    a.type = antennas[i].type;

                    b.r = antennas[j].r; 
                    b.c = antennas[j].c + (antennas[j].c - antennas[i].c);
                    b.type = antennas[i].type;
                } else if (antennas[i].r < antennas[j].r && antennas[i].c == antennas[j].c) {
                    // vertical
                    a.r = antennas[i].r - (antennas[j].r - antennas[i].r); 
                    a.c = antennas[i].c;
                    a.type = antennas[i].type;

                    b.r = antennas[j].r + (antennas[j].r - antennas[i].r); 
                    b.c = antennas[j].c;
                    b.type = antennas[i].type;
                } else {
                    printf("[Error] Incorrect antenna\n");
                }
                antinodes[antinode_index] = a;
                antinode_index++;
                antinodes[antinode_index] = b; 
                antinode_index++;
            }
        }
    }

    cant = 0;
    for (int i = 0; i < antinode_index; i++) {
        if (antinodes[i].r >= 0 && antinodes[i].r < rows &&
            antinodes[i].c >= 0 && antinodes[i].c < cols) {
            // in limits of the map
            for (int j = i + 1; j < antinode_index; j++) {
                if (antinodes[i].r == antinodes[j].r && antinodes[i].c == antinodes[j].c) {
                    // remove duplicates
                    cant--;
                    break;
                }
            }
            cant++;
        }
    }

    printf("Antinodes: %d\n", cant);
}

void part2(char map[H][W], int rows, int cols, antenna antennas[Q_ANTENNA], int antenna_index) {
    antenna antinodes[Q_ANTINODE];
    int antinode_index, cant;

    antinode_index = 0;
    for (int i = 0; i < antenna_index; i++) {
        for (int j = i + 1; j < antenna_index; j++) {
            if (antennas[i].type == antennas[j].type) {
                // same type of antenna
                antenna a;
                a.type = antennas[i].type;
                if (antennas[i].r < antennas[j].r && antennas[i].c < antennas[j].c) {
                    a.r = antennas[i].r;
                    a.c = antennas[i].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r = a.r - (antennas[j].r - antennas[i].r); 
                        a.c = a.c - (antennas[j].c - antennas[i].c); 
                    }

                    a.r = antennas[j].r;
                    a.c = antennas[j].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r = a.r + (antennas[j].r - antennas[i].r); 
                        a.c = a.c + (antennas[j].c - antennas[i].c);
                    }
                } else if (antennas[i].r < antennas[j].r && antennas[i].c > antennas[j].c) {
                    a.r = antennas[i].r;
                    a.c = antennas[i].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r = a.r - (antennas[j].r - antennas[i].r); 
                        a.c = a.c + (antennas[i].c - antennas[j].c); 
                    }

                    a.r = antennas[j].r;
                    a.c = antennas[j].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r = a.r + (antennas[j].r - antennas[i].r); 
                        a.c = a.c - (antennas[i].c - antennas[j].c);
                    }
                } else if (antennas[i].r == antennas[j].r && antennas[i].c < antennas[j].c) {
                    // horizontal
                    a.r = antennas[i].r;
                    a.c = antennas[i].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.c -= antennas[j].c - antennas[i].c; 
                    }

                    a.c = antennas[j].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.c += antennas[j].c - antennas[i].c;
                    }
                } else if (antennas[i].r < antennas[j].r && antennas[i].c == antennas[j].c) {
                    // vertical
                    a.r = antennas[i].r;
                    a.c = antennas[i].c;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r -= antennas[j].r - antennas[i].r; 
                    }

                    a.r = antennas[j].r;
                    
                    while (a.r >= 0 && a.r < rows && a.c >= 0 && a.c < cols) {
                        antinodes[antinode_index] = a;
                        antinode_index++;

                        a.r += antennas[j].r - antennas[i].r; 
                    }
                } else {
                    printf("[Error] Incorrect antenna\n");
                }
            }
        }
    }

    cant = 0;
    for (int i = 0; i < antinode_index; i++) {
        if (antinodes[i].r >= 0 && antinodes[i].r < rows &&
            antinodes[i].c >= 0 && antinodes[i].c < cols) {
            // in limits of the map
            for (int j = i + 1; j < antinode_index; j++) {
                if (antinodes[i].r == antinodes[j].r && antinodes[i].c == antinodes[j].c) {
                    // remove duplicates
                    cant--;
                    break;
                }
            }
            cant++;
        }
    }

    printf("Antinodes: %d\n", cant);
}

void print_map(char map[H][W], int rows, int cols) {
    printf("The number of rows is %d\n", rows);
    printf("The number of cols is %d\n", cols);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%c", map[r][c]);
        }
        printf("\n");
    }
}

void print_antennas(antenna antennas[Q_ANTENNA], int antenna_index) {
    printf("Antennas:\n");
    for (int i = 0; i < antenna_index; i++) {
        printf("%c=(%d, %d)\n", antennas[i].type, antennas[i].r, antennas[i].c);
    }
}

void print_antinodes(antenna antinodes[Q_ANTINODE], int antinode_index) {
    printf("Antinodes:\n");
    for (int i = 0; i < antinode_index; i++) {
        printf("%c=(%d, %d)\n", antinodes[i].type, antinodes[i].r, antinodes[i].c);
    }
}

void draw_antinodes(char map[H][W], int rows, int cols,
        antenna antinodes[Q_ANTINODE], int antinode_index) {
    for (int i = 0; i < 250; i++) {
        map[antinodes[i].r][antinodes[i].c] = '#';
    }
}
