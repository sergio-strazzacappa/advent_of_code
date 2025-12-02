#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 10

void solve(char direction, int distance, bool* skip);

int dial    = 50;
int passwd  = 0;

int main() {
    FILE *fp;
    char line[LINE_LEN];
    bool skip = false;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    printf("[DEBUG] dial=%d\n", dial);

    while (fgets(line, LINE_LEN, fp)) {
        // read each line

        // parse the input into <direction, distance>
        char direction = line[0];
        char buffer[LINE_LEN];
        strncpy(buffer, line + 1, strlen(line) - 1);
        buffer[strlen(line) - 1] = '\0';
        int distance = atoi(buffer);

        solve(direction, distance, &skip);
    }
}

void solve(char direction, int distance, bool* skip) {
    printf("[DEBUG] <%c, %d> Current dial=%d\n", direction, distance, dial);

    while (distance >= 100) {
        distance -= 100;
        passwd++;
    }

    printf("[DEBUG] Distance=%d, Passwd=%d\n", distance, passwd);

    if (direction == 'L') {
        dial -= distance;
    } else if (direction == 'R') {
        dial += distance;
    } else {
        printf("[ERROR] Incorrect direction\n");
    }

    if (dial == 0) {
        passwd++;
        printf("[DEBUG] dial==0 [%d]\n", passwd);
    } else if (dial > 99) {
            dial -= 100;
            passwd++;
            printf("[DEBUG] dial > 99 [%d]\n", passwd);
    } else if (dial < 0) {
            dial += 100;

            if (!(*skip)) {
                passwd++;
            }

            printf("[DEBUG] dial < 0 [%d]\n", passwd);
        }

    if (dial == 0) {
        *skip = true;
    } else {
        *skip = false;
    }

    printf("[DEBUG] Passwd=%d, Dial=%d\n\n", passwd, dial);
}
