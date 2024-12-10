#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 10000

void mult(int *i, char line[], long long int *sum);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    long long int sum;
    int mul_seq, do_seq, dont_seq, enabled;

    sum     = 0;
    enabled = 1;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[Error] can't open the file %s\n", INPUT_FILE);
        return 1;
    }

    while (fgets(line, LINE_LEN, fp)) {
        mul_seq     = 0;
        do_seq      = 0;
        dont_seq    = 0;
        printf("\n\n");
        for (int i = 0; i < strlen(line); i++) {
            /* MUL( */
            if (mul_seq == 0 && line[i] == 'm')             mul_seq++; // m
            else if (mul_seq == 1 && line[i] == 'u')        mul_seq++; // mu
            else if (mul_seq == 2 && line[i] == 'l')        mul_seq++; // mul
            else if (mul_seq == 3 && line[i] == '(')        mul_seq++; // mul(
            else if (mul_seq == 4 && enabled) {
                mult(&i, line, &sum);
                mul_seq = 0;
            }
            else if ((mul_seq == 1 && line[i] != 'u') || 
                    (mul_seq == 2 && line[i] != 'l')  ||
                    (mul_seq == 3 && line[i] != '('))       mul_seq = 0;

            /* DO() */
            if (do_seq == 0 && line[i] == 'd')              do_seq++; // d
            else if (do_seq == 1 && line[i] == 'o')         do_seq++; // do
            else if (do_seq == 2 && line[i] == '(') {
                // do(
                do_seq++;
                dont_seq = 0;
            }
            else if (do_seq == 3 && line[i] == ')' && !enabled) {
                // do()
                printf("Enable\n");
                enabled = 1;
                do_seq = 0;
            } else if ((do_seq == 1 && line[i] != 'o')    ||
                       (do_seq == 2 && line[i] != '(')    || 
                       (do_seq == 3 && line[i] != ')')) {
                printf("Reset do()\n");
                do_seq = 0;
            }
            
            /* DON'T() */
            if (dont_seq == 0 && line[i] == 'd')            dont_seq++; // d
            else if (dont_seq == 1 && line[i] == 'o')       dont_seq++; // do
            else if (dont_seq == 2 && line[i] == 'n') {
                // don
                dont_seq++;
                do_seq = 0;
            }
            else if (dont_seq == 3 && line[i] == '\'')      dont_seq++; // don'
            else if (dont_seq == 4 && line[i] == 't')       dont_seq++; // don't
            else if (dont_seq == 5 && line[i] == '(')       dont_seq++; // don't(
            else if (dont_seq == 6 && line[i] == ')' && enabled) {
                // don't()
                printf("Disable\n");
                enabled = 0;
                dont_seq = 0;
            } else if ((dont_seq == 1 && line[i] != 'o')  ||
                       (dont_seq == 2 && line[i] != 'n')  ||
                       (dont_seq == 3 && line[i] != '\'') ||
                       (dont_seq == 4 && line[i] != 't')  ||
                       (dont_seq == 5 && line[i] != '(')) {
                printf("Reset don't()\n");
                dont_seq = 0;
            }
        }
    }

    printf("Answer=%lld\n", sum);

    fclose(fp);

    return 0;
}

void mult(int *i, char line[], long long int *sum) {
    char op1[4], op2[4];
    int n1_len, n2_len, coma;

    n1_len  = 0;
    n2_len  = 0;
    coma    = 0;

    for (; *i < strlen(line); ++*i) {
        if (isdigit(line[*i]) && !coma && n1_len < 3) {
            // first operand
            op1[n1_len] = line[*i];
            n1_len++;
        } else if (isdigit(line[*i]) && coma && n2_len < 3) {
            // second operand
            op2[n2_len] = line[*i];
            n2_len++;
        } else if (line[*i] == ',' && n1_len > 0) {
            // coma, separator of operands
            op1[n1_len] = '\0';
            n1_len++;
            coma = 1;
        } else if (line[*i] == ')' && n1_len > 0 && n2_len > 0) {
            // end of operations
            op2[n2_len] = '\0';
            n2_len++;
            printf("%lld * %lld = %lld\n", atoll(op1), atoll(op2), atoll(op1) * atoll(op2));
            *sum += atoll(op1) * atoll(op2);
            printf("The sum is %lld\n", *sum);
            break;
        } else break;
    }
    --*i;
}
