#include <stdio.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    100
#define H           100
#define W           100

void solve(char map[H][W], int rows, int cols);
int search(char map[H][W], int rows, int cols, int y, int x, char current,
        int score);
int search_rating(char map[H][W], int rows, int cols, int y, int x, char current,
        int score);

void print_map(char map[H][W], int rows, int cols);

int main() {
    FILE *fp;
    char line[LINE_LEN], map[H][W];
    int rows, cols;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }
    
    rows = 0;
    cols = 0;
    while (fgets(line, LINE_LEN, fp)) {
        cols = 0;
        while (line[cols] != '\n') {
            map[rows][cols] = line[cols];
            cols++;
        }
        rows++;
    }
    
    solve(map, rows, cols);
    fclose(fp);
    return 0;
}

void solve(char map[H][W], int rows, int cols) {
    int score, rating;

    score = 0;
    rating = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] == '0') {
                // trailhead
                score += search(map, rows, cols, r, c, '0', 0);

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (map[i][j] == '#') {
                            // restore the map
                            map[i][j] = '9';
                        }
                    }
                }
                rating += search_rating(map, rows, cols, r, c, '0', 0);
            }
        }
    }
    printf("The score is: %d\n", score);
    printf("The rating is: %d\n", rating);
}

int search(char map[H][W], int rows, int cols, int y, int x, char current,
        int score) {

    if (current == '9') {
        // success
        map[y][x] = '#';
        return score + 1;
    }

    if (y > 0 && map[y - 1][x] == current + 1) {
        score = search(map, rows, cols, y - 1, x, current + 1, score);
    }
    if (x < cols - 1 && map[y][x + 1] ==  current + 1) {
        score = search(map, rows, cols, y, x + 1, current + 1, score);
    }
    if (y < rows - 1 && map[y + 1][x] == current + 1) {
        score = search(map, rows, cols, y + 1, x, current + 1, score);
    }
    if (x > 0 && map[y][x - 1] == current + 1) {
        score = search(map, rows, cols, y, x - 1, current + 1, score);
    }
    return score;
}

int search_rating(char map[H][W], int rows, int cols, int y, int x, char current,
        int score) {

    if (current == '9') {
        // success
        return score + 1;
    }

    if (y > 0 && map[y - 1][x] == current + 1) {
        score = search_rating(map, rows, cols, y - 1, x, current + 1, score);
    }
    if (x < cols - 1 && map[y][x + 1] ==  current + 1) {
        score = search_rating(map, rows, cols, y, x + 1, current + 1, score);
    }
    if (y < rows - 1 && map[y + 1][x] == current + 1) {
        score = search_rating(map, rows, cols, y + 1, x, current + 1, score);
    }
    if (x > 0 && map[y][x - 1] == current + 1) {
        score = search_rating(map, rows, cols, y, x - 1, current + 1, score);
    }
    return score;
}

void print_map(char map[H][W], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%c", map[r][c]);
        }
        printf("\n");
    }
}
