#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 150

long sum = 0;

int main() {
    FILE *fp;
    char line[LINE_LEN];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    while (fgets(line, LINE_LEN, fp)) {
        // read each line

        int max_value = 0;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\n') {
                break;
            }

            for (int j = i + 1; j < strlen(line); j++) {
                if (line[j] == '\n') {
                    break;
                }

                int value = (line[i] - '0') * 10 + line[j] - '0';

                if (value > max_value) {
                    max_value = value;
                }
            }
        }
        sum += max_value;
    }

    printf("[SOLUTION] %ld\n", sum);
}
