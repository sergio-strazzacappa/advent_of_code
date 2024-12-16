#include <stdio.h>

#define INPUT_FILE      "input.txt"
#define LINE_LEN        10000
#define MOVEMENT_LEN    30000
#define H               1000
#define W               1000

struct point {
    int x;
    int y;
};

void part1(char map[H][W], int rows, int cols, char movements[MOVEMENT_LEN],
    int movements_index, struct point me);
void part2(char map[H][W], int rows, int cols, char movements[MOVEMENT_LEN],
    int movements_index, struct point me);
int score(char map[H][W], int rows, int cols);
int score2(char map[H][W], int rows, int cols);
int is_move(char c);

void print_map(char map[H][W], int rows, int cols);
void print_movements(char movements[MOVEMENT_LEN], int movements_index);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    char map[H][W], map2[H][W], movements[MOVEMENT_LEN];
    int rows, cols, cols2, movements_index;
    struct point me, me2;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    rows = 0;
    cols = 0;
    cols2 = 0;
    movements_index = 0;

    while (fgets(line, LINE_LEN, fp)) {
        if (!is_move(line[0]) && line[0] != '\n') {
            cols = 0;
            cols2 = 0;
        }
        for (int c = 0; line[c] != '\n' && !is_move(line[0]); c++) {
            map[rows][c] = line[c];

            if (line[c] == '#') {
                map2[rows][cols2] = '#';
                map2[rows][cols2 + 1] = '#';
            }
            if (line[c] == 'O') {
                map2[rows][cols2] = '[';
                map2[rows][cols2 + 1] = ']';
            }
            if (line[c] == '.') {
                map2[rows][cols2] = '.';
                map2[rows][cols2 + 1] = '.';
            }
            if (line[cols] == '@') {
                me.y = rows;
                me.x = cols;
                me2.y = rows;
                me2.x = cols2;
                map2[rows][cols2] = '@';
                map2[rows][cols2 + 1] = '.';
            }
            if (line[c + 1] == '\n') {
                rows++;
            }
            cols = c + 1;
            cols2 += 2;
        }

        if (is_move(line[0])) {
            for (int i = 0; line[i] != '\n'; i++) {
                movements[movements_index] = line[i];
                movements_index++;
            }
        }
    }

    part1(map, rows, cols, movements, movements_index, me);
    part2(map2, rows, cols2, movements, movements_index, me2);

    printf("The score is %d\n", score(map, rows, cols));
    printf("The score is %d\n", score2(map2, rows, cols2));

    fclose(fp);
    return 0;
}

void part1(char map[H][W], int rows, int cols, char movements[MOVEMENT_LEN],
    int movements_index, struct point me) {

    for (int m = 0; m < movements_index; m++) {
        // iterate over the movements
        char move = movements[m];

        switch(move) {
            case '>':
                if (map[me.y][me.x + 1] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y][me.x + 1] = '@';
                    me.x = me.x + 1;
                } else if (map[me.y][me.x + 1] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int x = me.x + 2; x < cols; x++) {
                        if (map[me.y][x] == '#') {
                            // a wall, can't push the box
                            boxes = 0;
                            break;
                        } else if (map[me.y][x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        map[me.y][me.x] = '.';
                        map[me.y][me.x + 1] = '@';
                        map[me.y][me.x + 1 + boxes] = 'O';
                        me.x = me.x + 1;
                    }
                }
                break;
            case 'v':
                if (map[me.y + 1][me.x] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y + 1][me.x] = '@';
                    me.y = me.y + 1;
                } else if (map[me.y + 1][me.x] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int y = me.y + 2; y < rows; y++) {
                        if (map[y][me.x] == '#') {
                            // a wall, can't push the box
                            boxes = 0;
                            break;
                        } else if (map[y][me.x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        map[me.y][me.x] = '.';
                        map[me.y + 1][me.x] = '@';
                        map[me.y + 1 + boxes][me.x] = 'O';
                        me.y = me.y + 1;
                    }
                }
                break;
            case '<':
                if (map[me.y][me.x - 1] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y][me.x - 1] = '@';
                    me.x = me.x - 1;
                } else if (map[me.y][me.x - 1] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int x = me.x - 2; x >= 0; x--) {
                        if (map[me.y][x] == '#') {
                            // a wall, can't push the box
                            boxes = 0;
                            break;
                        } else if (map[me.y][x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        map[me.y][me.x] = '.';
                        map[me.y][me.x - 1] = '@';
                        map[me.y][me.x - 1 - boxes] = 'O';
                        me.x = me.x - 1;
                    }
                }
                break;
            case '^':
                if (map[me.y - 1][me.x] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y - 1][me.x] = '@';
                    me.y = me.y - 1;
                } else if (map[me.y - 1][me.x] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int y = me.y - 2; y >= 0; y--) {
                        if (map[y][me.x] == '#') {
                            // a wall, can't push the box
                            boxes = 0;
                            break;
                        } else if (map[y][me.x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        map[me.y][me.x] = '.';
                        map[me.y - 1][me.x] = '@';
                        map[me.y - 1 - boxes][me.x] = 'O';
                        me.y = me.y - 1;
                    }
                }
                break;
            default:
                printf("[ERROR] Incorrect move %c\n", move);
                break;
        }
    }
}

void part2(char map[H][W], int rows, int cols, char movements[MOVEMENT_LEN],
    int movements_index, struct point me) {

    for (int m = 0; m < movements_index; m++) {
        // iterate over the movements
        char move = movements[m];

        switch (move) {
            case '>':
                if (map[me.y][me.x + 1] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y][me.x + 1] = '@';
                    me.x = me.x + 1;
                } else if (map[me.y][me.x + 1] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int x = me.x + 3; x < cols; x += 2) {
                        if (map[me.y][x] == '#') {
                            // a wall, can't push the box
                            boxes = 0;
                            break;
                        } else if (map[me.y][x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        for (int i = boxes * 2; i > 0; i--) {
                            map[me.y][me.x + i + 1] = map[me.y][me.x + i];
                        }
                        map[me.y][me.x] = '.';
                        map[me.y][me.x + 1] = '@';
                        me.x = me.x + 1;
                    }
                }
                break;
            case 'v':
                if (map[me.y + 1][me.x] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y + 1][me.x] = '@';
                    me.y = me.y + 1;
                } else if (map[me.y + 1][me.x] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    struct point box_positions[1000], current, temp;
                    box_positions[0] = me;
                    int box_i = 1;
                    int wall = 0;

                    for (int i = 0; i < box_i && !wall; i++) {
                        current = box_positions[i];
                        if (map[current.y + 1][current.x] == '[') {
                            int in = 0;
                            temp.y = current.y + 1;
                            temp.x = current.x;
                            for (int j = 0; j < box_i; j++) {
                                if (box_positions[j].x == temp.x && box_positions[j].y == temp.y) {
                                    in = 1;
                                    break;
                                }
                            }
                            if (!in) {
                                box_positions[box_i] = temp;
                                box_i++;
                                temp.x = current.x + 1;
                                box_positions[box_i] = temp;
                                box_i++;
                            }
                        } else if (map[current.y + 1][current.x] == ']') {
                            int in = 0;
                            temp.y = current.y + 1;
                            temp.x = current.x - 1;
                            for (int j = 0; j < box_i; j++) {
                                if (box_positions[j].x == temp.x && box_positions[j].y == temp.y) {
                                    in = 1;
                                    break;
                                }
                            }
                            if (!in) {
                                box_positions[box_i] = temp;
                                box_i++;
                                temp.x = current.x;
                                box_positions[box_i] = temp;
                                box_i++;
                            }
                        } else if (map[current.y + 1][current.x] == '#') {
                            wall = 1;
                        }
                    }
                    if (!wall) {
                        for (int i = box_i - 1; i >= 0; i--) {
                            temp = box_positions[i];
                            map[temp.y + 1][temp.x] = map[temp.y][temp.x];
                            map[temp.y][temp.x] = '.';
                        }
                        me.y = me.y + 1;
                    }
                }
                break;
            case '<': 
                if (map[me.y][me.x - 1] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y][me.x - 1] = '@';
                    me.x = me.x - 1;
                } else if (map[me.y][me.x - 1] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    int boxes = 1;
                    for (int x = me.x - 3; x >= 0; x -= 2) {
                        if (map[me.y][x] == '#') {
                            // a wall, cant push the box
                            boxes = 0;
                            break;
                        } else if (map[me.y][x] == '.') {
                            // empty space, can push
                            break;
                        } else {
                            // another box
                            boxes++;
                        }
                    }
                    if (boxes > 0) {
                        // move the boxes
                        for (int i = boxes * 2; i > 0; i--) {
                            map[me.y][me.x - i - 1] = map[me.y][me.x - i];
                        }
                        map[me.y][me.x] = '.';
                        map[me.y][me.x - 1] = '@';
                        me.x = me.x - 1;
                    }
                }
                break;
            case '^':
                if (map[me.y - 1][me.x] == '.') {
                    // i can move
                    map[me.y][me.x] = '.';
                    map[me.y - 1][me.x] = '@';
                    me.y = me.y - 1;
                } else if (map[me.y - 1][me.x] == '#') {
                    // wall, i can't move
                    break;
                } else {
                    // box, try to push
                    struct point box_positions[1000], current, temp;
                    box_positions[0] = me;
                    int box_i = 1;
                    int wall = 0;

                    for (int i = 0; i < box_i && !wall; i++) {
                        current = box_positions[i];
                        if (map[current.y - 1][current.x] == '[') {
                            int in = 0;
                            temp.y = current.y - 1;
                            temp.x = current.x;
                            for (int j = 0; j < box_i; j++) {
                                if (box_positions[j].x == temp.x && box_positions[j].y == temp.y) {
                                    in = 1;
                                    break;
                                }
                            }
                            if (!in) {
                                box_positions[box_i] = temp;
                                box_i++;
                                temp.x = current.x + 1;
                                box_positions[box_i] = temp;
                                box_i++;
                            }
                        } else if (map[current.y - 1][current.x] == ']') {
                            int in = 0;
                            temp.y = current.y - 1;
                            temp.x = current.x - 1;
                            for (int j = 0; j < box_i; j++) {
                                if (box_positions[j].x == temp.x && box_positions[j].y == temp.y) {
                                    in = 1;
                                    break;
                                }
                            }
                            if (!in) {
                                box_positions[box_i] = temp;
                                box_i++;
                                temp.x = current.x;
                                box_positions[box_i] = temp;
                                box_i++;
                            }
                        } else if (map[current.y - 1][current.x] == '#') {
                            wall = 1;
                        }
                    }
                    if (!wall) {
                        for (int i = box_i - 1; i >= 0; i--) {
                            temp = box_positions[i];
                            map[temp.y - 1][temp.x] = map[temp.y][temp.x];
                            map[temp.y][temp.x] = '.';
                        }
                        me.y = me.y - 1;
                    }
                }
                break;
            default:
                printf("[ERROR] Incorrect move %c\n", move);
                break;
        }
    }
}

int score(char map[H][W], int rows, int cols) {
    int s = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] == 'O') {
                s += 100 * r + c;
            }
        }
    }
    return s;
}

int score2(char map[H][W], int rows, int cols) {
    int s = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] == '[') {
                s += 100 * r + c;
             }
         }
     }
     return s;
}

int is_move(char c) {
    return c == '>' || c == 'v' || c == '<' || c == '^';
}

void print_map(char map[H][W], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%c", map[r][c]);
        }
        printf("\n");
    }
}

void print_movements(char movements[MOVEMENT_LEN], int movements_index) {
    printf("[");
    for (int i = 0; i < movements_index; i++) {
        printf(" %c", movements[i]);
    }
    printf(" ]\n");
}
