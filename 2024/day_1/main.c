#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE  "input.txt"
#define N_ELEM      1000

int cmp(const void *p1, const void *p2);
void dump(int list[], int n);

int main() {
    FILE *fp;
    char line[100];
    int list1[N_ELEM], list2[N_ELEM], len, next, dif;
    long int sum, prod, score;
    
    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] File doesn't exists\n");
        return 1;
    }

    len = 0;
    next = 1;

    while (fgets(line, 100, fp)) { /* read each line of input */
        char *token;

        /* split each line by space character */
        token = strtok(line, " ");
        
        while (token != NULL) {
            if (next == 1) { /* insert token in the first list */
                list1[len] = atoi(token);
                next = 2;
            } else if (next == 2) { /* insert token in the second list */
                list2[len] = atoi(token);
                next = 1;
            }
            token = strtok(NULL, " ");
        }
        len++;
    }
    
    /* sort both list in ascendent order */
    qsort(list1, len, sizeof(int), cmp);
    qsort(list2, len, sizeof(int), cmp);
    
    sum = 0;
    for (int i = 0; i < len; i++) {
       dif = abs(list1[i] - list2[i]); 
       sum += dif;
    }

    //dump(list1, len);
    //dump(list2, len);

    printf("Total distance: %ld\n", sum);
    
    score = 0;
    for (int i = 0; i < len; i++) {
        prod = 0;
        for (int j = 0; j < len || list1[i] > list2[j]; j++) {
            if (list1[i] == list2[j]) {
                prod += list1[i];
            }
        }
        score += prod;
    }

    printf("Total score: %ld\n", score);

    fclose(fp);

    return 0;
}

int cmp(const void *p1, const void *p2) {
    return (*(int *)p1 - *(int *)p2);
}

void dump(int list[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d", list[i]);
        
        if (i < n - 1)
            printf(", ");
    }
    printf(" ]\n");
}
