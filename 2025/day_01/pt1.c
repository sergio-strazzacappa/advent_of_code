#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 10

void solve(char direction, int distance);

int dial    = 50;
int passwd  = 0;

int main() {
    FILE *fp;
    char line[LINE_LEN];

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

        solve(direction, distance);
    }
}

void solve(char direction, int distance) {
    printf("[DEBUG] <%c, %d>\n", direction, distance);

    if (direction == 'L') {
        dial -= distance;
    } else if (direction == 'R') {
        dial += distance;
    } else {
        printf("[ERROR] Incorrect direction\n");
    }

    dial = dial % 100;

    if (!dial)      passwd++;     

    printf("[DEBUG] dial=%d, passwd=%d\n", dial, passwd);
}
