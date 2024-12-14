#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define EPSILON     0.000001

/*
 * AX1 + BX2 = X
 * AY1 + BY2 = Y
 *
 * A = (X - BX2) / X1 -> first equation
 * 
 * (X - BX2)Y1 / X1 + BY2 = Y
 * (Y1X - Y1BX2) / X1 + BY2 = Y
 * [Y1X / X1] - [Y1BX2 / X1] + BY2 = Y
 * [Y1X / X1] - [Y1X2 / X1]B + BY2 = Y
 * -[Y1X2 / X1]B + BY2 = Y - [Y1X / X1]
 *  [Y1X2 / X1]B - BY2 = -Y + [Y1X / X1]
 *  [(Y1X2 / X1) - Y2]B = -Y + [Y1X / X1]
 *  B = [-Y + (Y1X / X1)] / [(Y1X2 / X1) - Y2] -> second ecuation
 */

typedef struct button {
    long double x;
    long double y;
    int token;
} button;

long double solve(button a, button b, long double x_prize, long double y_prize);
void print_point(button p);

int main() {
    FILE *fp;
    char line[LINE_LEN], *token;
    int line_number, field, problem;
    long double sum, x_prize, y_prize;
    button a, b;

    if ((fp = fopen(INPUT_FILE, "r")) == 0) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    line_number = 0;
    field = 0;
    a.token = 3;
    b.token = 1;
    sum = 0;
    problem = 0;
    while (fgets(line, LINE_LEN, fp)) {
        if (line[0] == '\n') continue;
        line_number++;
        token = strtok(line, "+=\n");
        field = 1;    
        while (token != NULL) {
            token = strtok(NULL, "+=,\n");
            field++;
            if (line_number % 3 == 1) {
                if (field == 2) a.x = atoi(token);
                if (field == 4) a.y = atoi(token);
            }
            if (line_number % 3 == 2) {
                if (field == 2) b.x = atoi(token);
                if (field == 4) b.y = atoi(token);
            }
            if (line_number % 3 == 0) {
                if (field == 2) {
                    x_prize = atoi(token) + 10000000000000;
                } 
                if (field == 4) {
                    y_prize = atoi(token) + 10000000000000;
                }
            }
        }
        if (line_number % 3 == 0) {
            problem++;
            sum += solve(a, b, x_prize, y_prize);
        }
    }
    printf("The sum is %.LF\n", sum);
    fclose(fp);
    return 0;
}

long double solve(button a, button  b, long double x_prize, long double y_prize) {
    long double solution, A, B;

    solution = 0;
    B = (-y_prize + (a.y * x_prize / a.x)) / ((a.y * b.x / a.x) - b.y);
    A = (x_prize - B * b.x) / a.x; 

    if (A >= 0 && fabsl(A - round(A)) < EPSILON && B >= 0 && fabsl(B - round(B)) < EPSILON)
        solution = a.token * A + b.token * B;

    return solution;
}

void print_point(button b) {
    printf("Button: (%.Lf, %.Lf, %d)\n", b.x, b.y, b.token);
}
