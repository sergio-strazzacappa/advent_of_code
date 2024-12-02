#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"
#define MAX_LINE 1000

void dump(int report[], int n);
int is_safe(int report[], int n, int bad_level);
void copy(int dst[], int src[], int exclude, int n);

int main() {
    FILE *fp;
    char line[MAX_LINE];
    int safe, line_number, index, report[30];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] File doesn't exist\n");
        return 1;
    }
    
    safe = 0;
    line_number = 0;

    while (fgets(line, MAX_LINE, fp)) {
        line_number++;
        printf("Line number %d ", line_number);
        char *token;

        token = strtok(line, " ");
        index = 0;

        while (token != NULL) {
            report[index] = atoi(token);
            index++;
            token = strtok(NULL, " ");
        }
        
        dump(report, index);
        safe += is_safe(report, index, 0);
        printf("Safe=%d\n", safe);
    }

    printf("Answer: %d\n", safe);

    return 0;
}

void dump(int report[], int n) {
    printf("[ ");

    for (int i = 0; i < n; i++) {
        printf("%d", report[i]);

        if (i < n - 1) printf(", ");
    }
    printf(" ]\n");
}

int is_safe(int report[], int n, int bad_level) {
    int direction, val_1, val_2;
    
    direction = -1;
    for (int i = 0; i < n - 1; i++) {
        val_1 = report[i];
        val_2 = report[i + 1];

        if (val_1 < val_2 && direction == -1) direction = 1; // asc
        if (val_1 > val_2 && direction == -1) direction = 2; // desc

        if (val_1 == val_2) {
            if (bad_level == 1) {
                return 0;
            } else {
                int new_report[30], r;
                copy(new_report, report, i, n);
                r = is_safe(new_report, n - 1, 1);
                if (r == 0) {
                    copy(new_report, report, i + 1, n);
                    r = is_safe(new_report, n - 1, 1);
                }
                if (r == 0) {
                    copy(new_report, report, i - 1, n);
                    r = is_safe(new_report, n - 1, 1);
                }
                return r;
            } 
        }

        if (direction == 1) {
            if (val_1 >= val_2 || val_2 - val_1 > 3 || val_2 - val_1 < 1) {
                if (bad_level == 0) {
                    int new_report[30], r;
                    copy(new_report, report, i, n);
                    r = is_safe(new_report, n - 1, 1);
                    if (r == 0) {
                        copy(new_report, report, i + 1, n);
                        r = is_safe(new_report, n - 1, 1);
                    }
                    if (r == 0) {
                        copy(new_report, report, i - 1, n);
                        r = is_safe(new_report, n - 1, 1);
                    }
                    return r;
                }
                return 0;
            }
        }

        if (direction == 2) {
            if (val_1 <= val_2 || val_1 - val_2 > 3 || val_1 - val_2 < 1) {
                printf("Not safe %d %d\n", val_1, val_2);
                if (bad_level == 0) {
                    int new_report[30], r;
                    copy(new_report, report, i, n);
                    r = is_safe(new_report, n - 1, 1);
                    if (r == 0) {
                        copy(new_report, report, i + 1, n);
                        r = is_safe(new_report, n - 1, 1);
                    }
                    if (r == 0) {
                        copy(new_report, report, i - 1, n);
                        r = is_safe(new_report, n - 1, 1);
                    }
                    return r;
                }
                return 0;
            }
        }
    }
    return 1;
}

void copy(int dst[], int src[], int exclude, int n) {
    for (int j = 0, k = 0; j < n; j++) {
        if (j != exclude) {
            dst[k] = src[j];
            k++;
        }
    }
}
