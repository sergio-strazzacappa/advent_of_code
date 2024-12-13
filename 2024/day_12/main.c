#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define H           1000
#define W           1000

typedef struct point {
    int x;
    int y;
    int compacted;
    char direction; // u = up, d = down, l = left, r = right
} point;

int region_price(char map[H][W], int rows, int cols, char region, int y, int x);
int dfs(char map[H][W], int rows, int cols, char region, int r, int c, int y,
        int x, int *area, int *perimeter, point perimeter_points[1000],
        int *perimeter_index);
void add_extern(point perimeter_points[1000], int *perimeter_index, int r,
        int c, int rows, int columns);
int calculate_size(point perimeter_points[1000], int perimeter_index, char map[H][W], int rows, int cols);
int same_size(point p, point q, char map[H][W], int rows, int cols);

int comp_rows(const void *p, const void *q);
int comp_cols(const void *p, const void *q);

void print_map(char map[H][W], int rows, int cols);
void print_perimeter(point perimeter_points[1000], int perimeter_index);

int main() {
    FILE *fp;
    char line[LINE_LEN], map[H][W];
    int rows, cols, total_price;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    rows = 0;
    cols = 0;
    while (fgets(line, LINE_LEN, fp)) {
        for (cols = 0; cols < strlen(line); cols++) {
            map[rows][cols] = line[cols];
        }
        rows++;
    }
    cols--;

    int n = 0;
    total_price = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] != '-') {
                n++;
                total_price += region_price(map, rows, cols, map[r][c], r, c);
            }
        }
    }

    printf("The total price is %d\n", total_price);

    fclose(fp);
    return 0;
}

int region_price(char map[H][W], int rows, int cols, char region, int y, int x) {
    int price, area, perimeter, perimeter_index, size;
    point perimeter_points[1000];

    area = 0;
    perimeter = 0;
    perimeter_index = 0 ;

    dfs(map, rows, cols, region, 0, 0, y, x, &area, &perimeter,
        perimeter_points, &perimeter_index);

    size = calculate_size(perimeter_points, perimeter_index, map, rows, cols);
    
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (map[r][c] == '#') {
                map[r][c] = '-';
            }
        }
    }

    price = area * size;

    return price;
}

// depth-first search
int dfs(char map[H][W], int rows, int cols, char region, int r, int c, int y,
        int x, int *area, int *perimeter, point perimeter_points[1000],
        int *perimeter_index) {

    if (region != map[y][x]) {
        if (map[y][x] == '#') {
            return 1;
        }
        point p = {x, y, 0, ' '};

        if (r < y) p.direction = 'd';
        if (r > y) p.direction = 'u';
        if (c < x) p.direction = 'r';
        if (c > x) p.direction = 'l';

        perimeter_points[*perimeter_index] = p;
        (*perimeter_index)++;
        return 0;
    }

    add_extern(perimeter_points, perimeter_index, y, x, rows, cols);
    (*area)++;
    (*perimeter) += 4;

    map[y][x] = '#'; // mark to compute perimeter

    if (y > 0) {
        *perimeter -= 
            dfs(map, rows, cols, region, y, x, y - 1, x, area, perimeter,
                perimeter_points, perimeter_index);
    }
    if (y < rows - 1) {
        *perimeter -= 
            dfs(map, rows, cols, region, y, x, y + 1, x, area,  perimeter,
                perimeter_points, perimeter_index);
    }
    if (x > 0) {
        *perimeter -= 
            dfs(map, rows, cols, region, y, x, y, x - 1, area, perimeter,
                perimeter_points, perimeter_index);
    }
    if (x < cols - 1) {
        *perimeter -=
            dfs(map, rows, cols, region, y, x, y, x + 1, area, perimeter,
                perimeter_points, perimeter_index);
    }

    return 1;
}

// add perimeter outside of the map
void add_extern(point perimeter_points[1000], int *perimeter_index, int r, int c, int rows, int cols) {
    int add = 1;
    if (r == 0) {
        for (int i = 0; i < *perimeter_index; i++) {
            if (perimeter_points[i].x == c && perimeter_points[i].y == r - 1) {
                // point already in
                add = 0;
                break;
            }
        }
        if (add) {
            point p = {c, r - 1, 0, ' '};
            perimeter_points[*perimeter_index] = p;
            (*perimeter_index)++;
        }
    }
    add = 1;
    if (r == rows - 1) {
        for (int i = 0; i < *perimeter_index; i++) {
            if (perimeter_points[i].x == c && perimeter_points[i].y == r + 1) {
                // point already in
                add = 0;
                break;
            }
        }
        if (add) {
            point p = {c, r + 1, 0, ' '};
            perimeter_points[*perimeter_index] = p;
            (*perimeter_index)++;
        }
    }
    add = 1;
    if (c == 0) {
        for (int i = 0; i < *perimeter_index; i++) {
            if (perimeter_points[i].x == c - 1 && perimeter_points[i].y == r) {
                // point already in
                add = 0;
                break;
            }
        }
        if (add) {
            point p = {c - 1, r, 0, ' '};
            perimeter_points[*perimeter_index] = p;
            (*perimeter_index)++;
        }
    }
    add = 1;
    if (c == cols - 1) {
        for (int i = 0; i < *perimeter_index; i++) {
            if (perimeter_points[i].x == c + 1 && perimeter_points[i].y == r) {
                // point already in
                add = 0;
                break;
            }
        }
        if (add) {
            point p = {c + 1, r, 0, ' '};
            perimeter_points[*perimeter_index] = p;
            (*perimeter_index)++;
        }
    }
}

int calculate_size(point perimeter_points[1000], int perimeter_index, char map[H][W], int rows, int cols) {
    int size;
    point *p, *q;

    // sort in row-order
    qsort(perimeter_points, perimeter_index, sizeof(point), comp_rows);
    size = perimeter_index;

    for (int i = 0; i < perimeter_index; i++) {
        p = perimeter_points + i;
        for (int j = i + 1; j < perimeter_index; j++) {
            q = perimeter_points + j;
            if (p->y == q->y && abs(p->x - q->x) == 1 && !q->compacted && same_size(*p, *q, map, rows, cols)) {
                size--;
                q->compacted = 1;
                p = q;
            }
        }
    }

    // sort in column order
    qsort(perimeter_points, perimeter_index, sizeof(point), comp_cols);
    for (int i = 0; i < perimeter_index; i++) {
        p = perimeter_points + i;
        for (int j = i + 1; j < perimeter_index; j++) {
            q = perimeter_points + j;
            if (p->x == q->x && abs(p->y - q->y) == 1 && !q->compacted && same_size(*p, *q, map, rows, cols)) {
                size--;
                q->compacted = 1;
                p = q;
            }
        }
    }
    return size;
}

int same_size(point p, point q, char map[H][W], int rows, int cols) {
    char region_p, region_q;
    int cond;

    region_p = map[p.y + 1][p.x];
    region_q = map[q.y + 1][q.x];
    cond = (region_p == '#' && region_q == '#');

    if ((cond && (p.direction == q.direction)) || (cond && q.direction == ' ')) {
        return 1;
    }

    region_p = map[p.y - 1][p.x];
    region_q = map[q.y - 1][q.x];
    cond = region_p == '#' && region_q == '#';

    if ((cond && (p.direction == q.direction)) || (cond && q.direction == ' ')) {
        return 1;
    }

    region_p = map[p.y][p.x + 1];
    region_q = map[q.y][q.x + 1];
    cond = region_p == '#' && region_q == '#';

    if ((cond && (p.direction == q.direction)) || (cond && q.direction == ' ')) {
        return 1;
    }

    region_p = map[p.y][p.x - 1];
    region_q = map[q.y][q.x - 1];
    cond = region_p == '#' && region_q == '#';

    if ((cond && (p.direction == q.direction)) || (cond && q.direction == ' ')) {
        return 1;
    }
    
    return 0;
}

// sort arrays by rows, then by columns
int comp_rows(const void *p, const void *q) {
    if (((point *)p)->y < ((point *)q)->y) {
        return -1;
    } else if (((point *)p)->y == ((point *)q)->y && ((point *)p)->x < ((point *)q)->x) {
        return -1;
    } 
    return 1;
}

// sort arrays by columns, then by rows
int comp_cols(const void *p, const void *q) {
    if (((point *)p)->x < ((point *)q)->x) {
        return -1;
    } else if (((point *)p)->x == ((point *)q)->x && ((point *)p)->y < ((point *)q)->y) {
        return -1;
    } 
    return 1;
}

void print_map(char map[H][W], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%c", map[r][c]);
        }
        printf("\n");
    }
}

void print_perimeter(point perimeter_points[1000], int perimeter_index) {
    printf("[");
    for (int i = 0; i < perimeter_index; i++) {
        point p = perimeter_points[i];
        printf(" (%d, %d, %c)", p.y, p.x, p.direction);
        
    }
    printf(" ]\n");
}
