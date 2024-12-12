#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE      "input.txt"
#define LINE_LEN        100
#define BLINKS          75
#define CACHE_SIZE      200000

typedef struct cache_data {
    unsigned long long int token;
    int blink;
    unsigned long long int result;
} cache_data;

int cache_index;

unsigned long long int count(char *token, int blinks, cache_data cache[CACHE_SIZE]);
unsigned long long int search(cache_data cache[CACHE_SIZE], unsigned long long int token, int blinks);
void print_cache(cache_data[CACHE_SIZE]);
void count_old(char *token, int blinks, long long int *stones);

int main() {
    FILE *fp;
    char line[LINE_LEN], *token;
    unsigned long long int stones;
    cache_data cache[CACHE_SIZE];
    
    
    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    stones = 0;
    while (fgets(line, LINE_LEN, fp)) {
        token = strtok(line, " \n");

        while (token != NULL) {
            cache_index = 0;
            stones += count(token, BLINKS, cache);
            token = strtok(NULL, " \n");
        }
    }
    printf("The number of stones is %lld\n", stones);
    fclose(fp);
    return 0;
}

unsigned long long int count(char *token, int blinks, cache_data cache[CACHE_SIZE]) {
    if (blinks == 0) {
        return 1;
    }
    if (atoll(token) == 0) {
        unsigned long long int result = search(cache, 1, blinks - 1);

        if (result == -1) {
            result = count("1", blinks - 1, cache);
            cache_data c;
            c.token = 1;
            c.blink = blinks - 1;
            c.result = result;
            cache[cache_index] = c;
            cache_index++;
        }
        return result;
    }

    if (strlen(token) % 2 == 0) {
        // even digits
        char left[LINE_LEN], right[LINE_LEN];
        int i, j;

        j = 0;
        for (i = 0; i < strlen(token); i++) {
            if (i < strlen(token) / 2) {
                left[i] = token[i];
            } else {
                left[i] = '\0';
                if (token[i] == '0' && j == 0) {
                    continue;
                }
                right[j] = token[i];
                j++;
            }
        }
        if (j == 0) {
            right[j] = '0';
            j++;
        }

        right[j] = '\0';

        unsigned long long int result1 = search(cache, atoll(left), blinks - 1);
        unsigned long long int result2 = search(cache, atoll(right), blinks - 1);

        if (result1 == -1) {
            result1 = count(left, blinks - 1, cache);
            cache_data c;
            c.token = atoll(left);
            c.blink = blinks - 1;
            c.result = result1;
            cache[cache_index] = c;
            cache_index++;
        }
        if (result2 == -1) {
            result2 = count(right, blinks - 1, cache);
            cache_data c;
            c.token = atoll(right);
            c.blink = blinks - 1;
            c.result = result2;
            cache[cache_index] = c;
            cache_index++;
        }
        
        return result1 + result2;
    }
    
    char new_token[LINE_LEN];
    sprintf(new_token, "%lld", atoll(token) * 2024);

    unsigned long long int result = search(cache, atoll(new_token), blinks - 1);

    if (result == -1) {
        result = count(new_token, blinks - 1, cache);
        cache_data c;
        c.token = atoll(new_token);
        c.blink = blinks - 1;
        c.result = result;
        cache[cache_index] = c;
        cache_index++;
    }
    return result;
}

unsigned long long int search(cache_data cache[CACHE_SIZE], unsigned long long int token, int blinks) {
    for (int i = 0; i < cache_index; i++) {
        if (token == cache[i].token && blinks == cache[i].blink) {
            return cache[i].result;
        }
    }
    return -1;
}

void print_cache(cache_data cache[CACHE_SIZE]) {
    printf("\nCACHE: \n");
    for (int i = 0; i < cache_index; i++) {
        printf("[%lld, %d, %lld]\n", cache[i].token, cache[i].blink, cache[i].result);
    }
    printf("\n");
}

void count_old(char *token, int blinks, long long int *stones) {
    if (blinks == BLINKS ) {
        (*stones)++;
        return;
    }

    if (atoll(token) == 0) {
        count_old("1", blinks + 1, stones);
        return;
    }

    if (strlen(token) % 2 == 0) {
        // even digits
        char left[LINE_LEN], right[LINE_LEN];
        int i, j;

        j = 0;
        for (i = 0; i < strlen(token); i++) {
            if (i < strlen(token) / 2) {
                left[i] = token[i];
            } else {
                left[i] = '\0';
                if (token[i] == '0' && j == 0) {
                    continue;
                }
                right[j] = token[i];
                j++;
            }
        }
        if (j == 0) {
            right[j] = '0';
            j++;
        }

        right[j] = '\0';

        count_old(left, blinks + 1, stones);
        count_old(right, blinks + 1, stones);
    } else {
        char new_token[LINE_LEN];
        sprintf(new_token, "%lld", atoll(token) * 2024);
        count_old(new_token, blinks + 1, stones);
    }
}
