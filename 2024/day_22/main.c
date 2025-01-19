#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    10
#define ITERATIONS  2000
#define MOD         16777216
#define NUMBER_SIZE 2173
#define SEQ_NUMBER  130321

long part1(int number);
long part2(int numbers[NUMBER_SIZE], int numbers_index);
long next_secret_number(long number);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    long sum_part1, sum_part2;
    int numbers[NUMBER_SIZE], numbers_index;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open the file %s\n", INPUT_FILE);
        return 1;
    }

    sum_part1 = 0;
    sum_part2 = 0;
    numbers_index = 0;

    while (fgets(line, LINE_LEN, fp)) {
        numbers[numbers_index++] = atoi(line);
        sum_part1 += part1(atoi(line));
    }

    sum_part2 = part2(numbers, numbers_index);
 
    printf("The result of part 1 is %ld\n", sum_part1);
    printf("The result of part 2 is %ld\n", sum_part2);

    fclose(fp);
    return 0;
}

long part1(int number) {
    for (int i = 0; i < ITERATIONS; i++) {
        number = next_secret_number(number);
    }
    return number;
}

long part2(int numbers[NUMBER_SIZE], int numbers_index) {
    int buyers[ITERATIONS], buyers_index, seq[4], seq_coded, found,
        seq_tested[SEQ_NUMBER], seq_tested_index, map[SEQ_NUMBER][2],
        map_index, max_sum;

    map_index = 0;
    max_sum = 0;
    
    for (int i = 0; i < numbers_index; i++) {
        long n = numbers[i];
        buyers_index = 0;
        seq_tested_index = 0;
        buyers[buyers_index++] = n % 10;

        // saves every price
        for (int j = 0; j < ITERATIONS; j++) {
            n = next_secret_number(n);
            buyers[buyers_index++] = n % 10;
        }

        // get every sequence and save it coded
        for (int j = 0; j < buyers_index - 4; j++) {
            int a = buyers[j];
            int b = buyers[j + 1];
            int c = buyers[j + 2];
            int d = buyers[j + 3];
            int e = buyers[j + 4];

            seq[0] = b - a;
            seq[1] = c - b;
            seq[2] = d - c;
            seq[3] = e - d;

            seq_coded = 0;
            for (int k = 3; k >= 0; k--) {
                if (seq[3 - k] < 0) seq_coded += pow(10, k * 2) * -seq[3 - k];
                else                seq_coded += pow(10, k * 2 + 1) * seq[3 - k];
            }

            // check if the sequence is already saved
            found = 0;
            for (int k = 0; k < seq_tested_index; k++) {
                if (seq_tested[k] == seq_coded) {
                    found = 1; 
                    break;
                }
            }

            // if it is, go to the next iteration
            if (found) continue;
            
            // save the sequence
            seq_tested[seq_tested_index++] = seq_coded;

            // check if the sequence is already saved in the map
            // if it is, add it the current price
            found = 0;
            for (int k = 0; k < map_index; k++) {
                if (map[k][0] == seq_coded) {
                    map[k][1] += e;
                    found = 1;
                    break;
                }
            }
            // if not add it to the map
            if (!found) {
                map[map_index][0] = seq_coded;
                map[map_index][1] = e;
                map_index++;
            }
        }
    }

    for (int i = 0; i < map_index; i++) {
        if (map[i][1] > max_sum) {
            max_sum = map[i][1];
        }
    }
    return max_sum;
}

// calculate the next secret number
long next_secret_number(long number) {
    number = number ^ number * 64 % MOD;
    number = number ^ number / 32 % MOD;
    number = number ^ number * 2048 % MOD;

    return number;
}
