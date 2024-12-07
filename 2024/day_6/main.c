#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define H           1000
#define W           1000

/*
 * ^ = 1
 * > = 2
 * v = 3
 * < = 4
 */

typedef struct player {
    int x;
    int y;
    int dir;
} player;

void part1(char map[H][W], int h, int w);
void part2(char map[H][W], int h, int w);
void move(char map[H][W], int h, int w, player *p, int *diff_positions);

player get_pos(char map[H][W], int h, int w); 

void dump(char map[H][W], int h, int w);
void print_player(player p);

int main() {
    FILE *fp;
    char line[LINE_LEN], map1[W][H], map2[W][H];
    int x, y;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }
    
    x = 0;
    y = 0;

    while (fgets(line, LINE_LEN, fp)) {
        for (x = 0; x < strlen(line); x++) {
            if (line[x] != '\n') {
                map1[x][y] = line[x];
                map2[x][y] = line[x];
            }
        }
        y++;
    }
    x--; // \n

    part1(map1, y, x);
    part2(map2, y, x);

    fclose(fp);

    return 0;
}

void part1(char map[H][W], int h, int w) {
    int in, diff_positions;
    player p;

    diff_positions = 1;
    p = get_pos(map, h, w);
    in = (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h);

    while (in) {
        map[p.x][p.y] = 'X';

        move(map, h, w, &p, &diff_positions); 
        in = (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h);
    }

    diff_positions--;
    printf("The number of different positions is %d\n", diff_positions);
}

void part2(char map[H][W], int h, int w) {
    int in, turns, loops, _;
    player p;

    loops = 0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map[x][y] == '.') {
                map[x][y] = '#';
                // test cycle
                turns = 0;
                p = get_pos(map, h, w);
                in = (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h);
                while (in && turns < h * w * 4) {
                    move(map, h, w, &p, &_); 
                    in = (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h);
                    turns++;
                }
                if (turns == h * w * 4) loops++;
                map[x][y] = '.';
            }
        }
    }

    printf("The number of loops is %d\n", loops);
}

void move(char map[H][W], int h, int w, player *p, int *diff_positions) {
    int move = 0;

    if (p->dir == 1) {
        if (map[p->x][(p->y) - 1] == '#') p->dir = 2;
        else {
            move = 1;
            (p->y)--;
            (*diff_positions)++;
        }
    } else if (p->dir == 2) {
        if (map[(p->x) + 1][p->y] == '#') p->dir = 3;
        else {
            move = 1;
            (p->x)++;
            (*diff_positions)++;
        }
    } else if (p->dir == 3) {
        if (map[p->x][(p->y) + 1] == '#') p->dir = 4;
        else {
            move = 1;
            (p->y)++;
            (*diff_positions)++;
        }
    } else if (p->dir == 4) {
        if (map[(p->x) - 1][p->y] == '#') p->dir = 1;
        else {
            move = 1;
            (p->x)--;
            (*diff_positions)++;
        }
    }

    if (map[p->x][p->y] == 'X' && move) {
        (*diff_positions)--;
    } 
}

player get_pos(char map[H][W], int h, int w) {
    player p;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (map[x][y] == '^' || map[x][y] == '>' || 
                map[x][y] == 'v' || map[x][y] == '<') {
                p.x = x;
                p.y = y;
            }

            if (map[x][y] == '^')   p.dir = 1;
            if (map[x][y] == '>')   p.dir = 2;
            if (map[x][y] == 'v')   p.dir = 3;
            if (map[x][y] == '<')   p.dir = 4;
        }
    }
    return p;
}

void dump(char map[H][W], int h, int w) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++){
            printf("%c", map[x][y]);
        }
        printf("\n");
    }
}

void print_player(player p) {
    printf("(%d, %d, %d)\n", p.x, p.y, p.dir);
}
