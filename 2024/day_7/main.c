#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"

enum ops {
    Add             = 1,
    Multiply        = 2,
    Concatenation   = 3
};

long long int solve(long long int result, int operands[], int index);
int test(long long result, int operands[], int operators[], int n);

void dump(int a[], int n);

int main() {
    FILE *fp;
    char line[1000], *token;
    int index, operands[1000];
    long long int result, sum;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    sum = 0;

    while (fgets(line, 1000, fp)) {
        index = 0;
        token = strtok(line, ":");
        result = atoll(token);

        while (1) {
            token = strtok(NULL, " ");
            if (token == NULL) break;
            operands[index] = atoi(token);
            index++;
        }
        sum += solve(result, operands, index);
    }

    printf("The answer is %lld\n", sum);

    return 0;
}

long long int solve(long long int result, int operands[], int index) {
    int operators[index - 1], failed;
    //printf("Testing: %lld\n", result);
    
    for (int i = 0; i < index - 1; i++) {
        operators[i] = Add;
    }
    
    while (1) {
        failed = 1;
        //printf("Operators: \n");
        //dump(operators, index - 1);

        if (test(result, operands, operators, index)) {
            //printf("Success\n");
            return result;
        }

        for (int i = 0; i < index - 1; i++) {
            if (operators[i] != Concatenation) {
                failed = 0;
                break;
            }
        }

        if (failed) break;

        for (int i = index - 2; i >= 0; i--) {
            if (operators[i] == Add) {
                operators[i] = Multiply;
                break;
            } else if (operators[i] == Multiply) {
                operators[i] = Concatenation;
                break;
            } else if (operators[i] == Concatenation) {
                operators[i] = Add;
            }
        }
    }
    //printf("Failed\n");
    return 0;
}

// test if the current sequence of operators is ok
int test(long long int result, int operands[], int operators[], int n) {
    long long int r;
    char *buf1, *buf2;

    buf1 = malloc(sizeof(long long int) * sizeof(char));
    buf2 = malloc(sizeof(int) * sizeof(char));

    r = operands[0];
    
    for (int i = 1; i < n; i++) {
        //printf("r: %lld\n", r);
        if (operators[i - 1] == Add)
            r += operands[i];
        
        if (operators[i - 1] == Multiply)
            r *= operands[i];

        if (operators[i - 1] == Concatenation) {
            sprintf(buf1, "%lld", r);
            sprintf(buf2, "%d", operands[i]);
            r = atoll(strcat(buf1, buf2));
        }
    }
    return (result == r) ? result : 0;
} 

void dump(int a[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i < n - 1) printf(" - "); 
    }
    printf(" ]\n");
}
