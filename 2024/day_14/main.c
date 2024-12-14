#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define H           103    // 7 example, 103 input
#define W           101    // 11 example, 101 input
#define MAX_ROBOTS  1000
#define TURNS       100

struct robot {
    int px;
    int py;
    int vx;
    int vy;
};

void simulate (struct robot robots[MAX_ROBOTS], int robots_index, int map[H][W]);
void move (struct robot *r, int map[H][W]);
void count(struct robot robots[MAX_ROBOTS], int robots_index);

void print_robots(struct robot robots[MAX_ROBOTS], int robots_index);
void print_map(int map[H][W]);

int main() {
    FILE *fp;
    char line[LINE_LEN], *token;
    int field, robots_index, map[H][W];
    struct robot robots[MAX_ROBOTS];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            map[y][x] = 0;
        }
    }

    while (fgets(line, LINE_LEN, fp)) {
        field = 0;
        struct robot r;
        token = strtok(line, "=, \n");
        while (token != NULL) {
            field++;
            if (field == 2) r.px = atoi(token);
            if (field == 3) r.py = atoi(token);
            if (field == 5) r.vx = atoi(token);
            if (field == 6) r.vy = atoi(token);
            token = strtok(NULL, "=, \n"); 
        }
        map[r.py][r.px] = map[r.py][r.px] + 1;
        robots[robots_index] = r;
        robots_index++;
    }

    simulate(robots, robots_index, map);
    count(robots, robots_index);

    fclose(fp);
    return 0;
}

void simulate(struct robot robots[MAX_ROBOTS], int robots_index, int map[H][W]) {
    int turns = 0;

    while (turns < 7847) {
        for (int i = 0; i < robots_index; i++) {
            move(&robots[i], map);
        }
        turns++;
    }
    print_map(map);
}

void move(struct robot *r, int map[H][W]) {
    map[r->py][r->px] = map[r->py][r->px] - 1;
    
    if (r->py + r->vy < 0)          r->py = H - abs(r->py + r->vy);
    else if (r->py + r->vy > H - 1) r->py = r->py + r->vy - H;
    else                            r->py += r->vy;

    if (r->px + r->vx < 0)          r->px = W - abs(r->px + r->vx);
    else if (r->px + r->vx > W - 1) r->px = r->px + r->vx - W; 
    else                            r->px += r->vx;
    
    map[r->py][r->px] = map[r->py][r->px] + 1;
}

void count(struct robot robots[MAX_ROBOTS], int robots_index) {
    int c1, c2, c3, c4;

    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    for (int i = 0; i < robots_index; i++) {
        struct robot r = robots[i];
        if (r.px < W / 2 && r.py < H / 2)   c1++;
        if (r.px > W / 2 && r.py < H / 2)   c2++;
        if (r.px < W / 2 && r.py > H / 2)   c3++;
        if (r.px > W / 2 && r.py > H / 2)   c4++;
    }
    printf("Safety %d\n", c1 * c2 * c3 * c4);
}

void print_robots(struct robot robots[MAX_ROBOTS], int robot_index) {
    printf("[");
    for (int i = 0; i < robot_index; i++) {
        printf(" (%d, %d)", robots[i].px, robots[i].py);
    }
    printf(" ]\n");
}

void print_map(int map[H][W]) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
           printf("%c", map[y][x] == 0 ? '-' : '#'); 
        }
        printf("\n");
    } 
}
