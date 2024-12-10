#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000
#define N_RULES     10000

struct rule {
    int x;
    int y;
};

void print_rules(struct rule rules[N_RULES], int n);
void print_updates(int updates[1000], int n);
int part1(struct rule rules[N_RULES], int n, int updates[1000], int m);
int part2(struct rule rules[N_RULES], int n, int updates[1000], int m);
int check(int current_update, int update, struct rule rules[N_RULES], int n);
void swap(int i, int j, int updates[1000], int m);

int main() {
    FILE *fp;
    char line[LINE_LEN], *token;
    struct rule rules[N_RULES];
    int read_rule, rules_index, updates[1000], updates_index;
    long long int part1_sum, part2_sum;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }
    
    read_rule = 1;
    rules_index = 0;
    updates_index = 0;
    part1_sum = 0;
    part2_sum = 0;

    while(fgets(line, LINE_LEN, fp)) {
        if (read_rule) {
            struct rule r;
            token = strtok(line, "|");
            r.x = atoi(token);

            if (atoi(token) != 0) {
                token = strtok(NULL, "|");
                r.y = atoi(token);
                rules[rules_index] = r;
                rules_index++;
            } else {
                read_rule = 0;
            }
        } else {
            token = strtok(line, ",");

            while (token != NULL && atoi(token) != 0) {
                updates[updates_index] = atoi(token);
                updates_index++;
                token = strtok(NULL, ",");
            }
            //print_updates(updates, updates_index);
            part1_sum += part1(rules, rules_index, updates, updates_index);
            part2_sum += part2(rules, rules_index, updates, updates_index);
            updates_index = 0;
        }
    }

    //print_rules(rules, rules_index);
    printf("The part 1 sum is %lld\n", part1_sum);
    printf("The part 2 sum is %lld\n", part2_sum);

    fclose(fp);

    return 0;
}

void print_rules(struct rule rules[N_RULES], int n) {
    printf("RULES:\n");
    for (int i = 0; i < n; i++) {
        printf("%d|%d\n", rules[i].x, rules[i].y);
    }
}

void print_updates(int updates[1000], int n) {
    printf("UPDATES:");
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d", updates[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

int part1(struct rule rules[N_RULES], int n, int updates[1000], int m) {
    int current_update, correct;

    correct = 1;
    for (int i = 0; i < m; i++) {
        current_update = updates[i];

        for (int j = i + 1; j < m; j++) {
            correct = check(current_update, updates[j], rules, n);
            if (!correct) {
                return 0;
            }
        }
    }

    if (correct) return updates[m / 2];

    return 0;
}

int part2(struct rule rules[N_RULES], int n, int updates[1000], int m) {
    int current_update, correct, show;

    correct = 1;
    show = 0;

    for (int i = 0; i < m; i++) {
        current_update = updates[i];

        for (int j = i + 1; j < m; j++) {
            correct = check(current_update, updates[j], rules, n);
            if (!correct) {
                //printf("swap\n");
                swap(i, j, updates, m);
                //print_updates(updates, m);
                i = -1;
                show = 1;
                break;
            }
        }
    }
    if (show) return updates[m / 2];
    return 0;
}

int check(int current_update, int update, struct rule rules[N_RULES], int n) {
    //printf("Check %d and %d\n", current_update, update);
    for (int i = 0; i < n; i++) {
        // iterate over the rules
        if (rules[i].x == update && rules[i].y == current_update) {
            //printf("INCORRECT\n");
            return 0;
        }
    }
    return 1;
}

void swap(int i, int j, int updates[1000], int m) {
    //printf("Swap %d with %d\n", updates[i], updates[j]);
    int temp = updates[i];
    updates[i] = updates[j];
    updates[j] = temp;
}
