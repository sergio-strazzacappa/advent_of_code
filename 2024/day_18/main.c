#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define INPUT_FILE      "input.txt"
#define LINE_LEN        20
#define H               71
#define W               71
#define NUMBER_BYTES    1024
#define MAX_BYTES       8192

typedef struct point {
    int x;
    int y;
} Point;

int solve(char memory[H][W]);
void move(Point current, Point m, int costs[H][W], Point left[H * W], int *left_index);
void fill_memory(char memory[H][W], Point bytes[NUMBER_BYTES]);
int point_equal(Point p, Point q);

void print_bytes(Point bytes[], int index);
void print_memory(char memory[H][W]);
void print_costs(int costs[H][W]);

int main() {
    FILE *fp;
    char line[LINE_LEN], memory[H][W];
    Point bytes[NUMBER_BYTES], next_bytes[MAX_BYTES];
    int i, next_bytes_index;

    if ((fp = fopen(INPUT_FILE, "r")) == 0) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    i = 0;
    next_bytes_index = 0;

    while (fgets(line, LINE_LEN, fp)) {
        Point p;

        p.x = atoi(strtok(line, ",\n"));
        p.y = atoi(strtok(NULL, ",\n"));

        if (i < NUMBER_BYTES) {
            bytes[i++] = p;
        } else {
            next_bytes[next_bytes_index++] = p;
        } 
    }

    fill_memory(memory, bytes);

    int j = 0;
    do {
        int solution = solve(memory);
        if (solution == -1) {
            printf("Point (%d, %d)\n", next_bytes[j - 1].x, next_bytes[j - 1].y);
            break;
        }
        memory[next_bytes[j].y][next_bytes[j].x] = '#';
        j++;
    } while(j < next_bytes_index);

    //print_bytes(bytes, i);
    //print_bytes(next_bytes, next_bytes_index);
    //print_memory(memory);

    fclose(fp);
    return 0;
}

int solve(char memory[H][W]) {
    int costs[H][W], left_index, solution;
    Point start, end, left[H * W], m;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (memory[y][x] == '#') {
                costs[y][x] = -1;
                continue;
            }
            costs[y][x] = INT_MAX;
        }
    }
    start.x = 0;
    start.y = 0;
    end.x = W - 1;
    end.y = H - 1;
    left_index = 0;
    solution = -1;

    costs[start.y][start.x] = 0;
    left[left_index++] = start;

    while (left_index > 0) {
        // compute the lowest cost node
        int min = INT_MAX;
        int index = -1;

        for (int i = 0; i < left_index; i++) {
            Point p = left[i];
            int c = costs[p.y][p.x];

            if (c < min) {
                min = c;
                index = i;
            }
        }
        Point current = left[index];

        // remove the current node from the left list
        for (int i = index; i < left_index - 1; i++) {
            left[i] = left[i + 1];
        }
        left_index--;

        if (point_equal(current, end)) {
            // goal found
            printf("Found a solution with cost %d\n", costs[end.y][end.x]);
            solution = costs[end.y][end.x];
            continue;
        }

        // right
        m = (Point){ 1, 0 };
        move(current, m, costs, left, &left_index);

        // down
        m = (Point){ 0, 1 };
        move(current, m, costs, left, &left_index);

        // left
        m = (Point){ -1, 0 };
        move(current, m, costs, left, &left_index);

        // up
        m = (Point){ 0, -1 };
        move(current, m, costs, left, &left_index);
    }
    return solution;
}

void move(Point current, Point m, int costs[H][W], Point left[H * W], int *left_index) {
    Point next = { current.x + m.x, current.y + m.y };

    if (next.x >= 0 && next.x <= W - 1 && next.y >= 0 && next.y <= H - 1) {
        if (costs[next.y][next.x] != -1 && costs[current.y][current.x] + 1 < costs[next.y][next.x]) {
            costs[next.y][next.x] = costs[current.y][current.x] + 1;
            left[(*left_index)++] = next;
        }
    } 
}

void fill_memory(char memory[H][W], Point bytes[NUMBER_BYTES]) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            memory[y][x] = '.';
        }
    }
    for (int i = 0; i < NUMBER_BYTES; i++) {
        Point b = bytes[i];
        memory[b.y][b.x] = '#';
    }
}

int point_equal(Point p, Point q) {
    return (p.x == q.x && p.y == q.y);
}

void print_bytes(Point bytes[], int index) {
    for (int i = 0; i < index; i++) {
        printf("%d, %d\n", bytes[i].x, bytes[i].y);
    }
}

void print_memory(char memory[H][W]) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            printf("%c", memory[y][x]);
        }
        printf("\n");
    }
}

void print_costs(int costs[H][W]) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            printf("%010d ", costs[y][x]);
        }
        printf("\n");
    }
}

