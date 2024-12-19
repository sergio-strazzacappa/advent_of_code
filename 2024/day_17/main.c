#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    1000

/*
 * Operations
 * 0) A = A / pow(2, op_combo)
 * 1) B = B ^ op_literal
 * 2) B = op_combo % 8
 * 3) jump op_literal
 * 4) B = B ^ C
 * 5) print(op_combo % 8)
 * 6) B = A / pow(2, op_combo)
 * 7) C = A / pow(2, op_combo)
 */

long long int A, B, C, IP;
char *program;

void part1();
void part2();

long long int combo(int operand);
 
void adv(int operand);
void bxl(int operand);
void bst(int operand);
void jnz(int operand);
void bxc(int operand);
long long int out(int operand);
void bdv(int operand);
void cdv(int operand);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    int line_number;

    if ((fp = fopen(INPUT_FILE, "r")) == 0) {
        printf("[ERROR] Can't open the file %s\n", INPUT_FILE);
        return 1;
    }

    line_number = 1;
    program = (char *)malloc(sizeof(char) * LINE_LEN);

    while (fgets(line, LINE_LEN, fp)) {
        if (line_number == 1) {
            strtok(line, ":");
            A = atoi(strtok(NULL, " :\n"));
        }
        if (line_number == 2) {
            strtok(line, ":");
            B = atoi(strtok(NULL, " :\n"));
        }
        if (line_number == 3) {
            strtok(line, ":");
            C = atoi(strtok(NULL, " :\n"));
        }
        if (line_number == 5) {
            strtok(line, ":");
            strcpy(program, strtok(NULL, " :\n"));
        }
        line_number++;
    } 

    IP = 0;
    part1();
    part2();

    free(program);
    fclose(fp);

    return 0;
}

void part1() {
    int comma = 0;
    while (IP * 2 < strlen(program)) {
        int opcode = program[IP * 2] - '0';
        int operand = program[IP * 2 + 2] - '0';

        switch(opcode) {
            case 0: 
                adv(operand);
                break;
            case 1:
                bxl(operand);
                break;
            case 2:
                bst(operand);
                break;
            case 3:
                jnz(operand);
                break;
            case 4:
                bxc(operand);
                break;
            case 5:
                if (comma) {
                    printf(",");
                }
                printf("%lld", out(operand));
                comma = 1;
                break;
            case 6:
                bdv(operand);
                break;
            case 7:
                cdv(operand);
                break;
            default:
                printf("Incorrect opcode\n");
                break;
        }
    }
    printf("\n");
}

void part2() {
    int comma, position;
    char *output, number[3];
    long long int a, b, c;

    position = 15;
    IP = 0;
    a = pow(8, position);
    b = B;
    c = C;
    output = (char *)malloc(sizeof(char) * LINE_LEN);

    while (1) {
        strcpy(output, "");
        A = a;
        B = b;
        C = c;
        IP = 0;
        comma = 0;

        while (IP * 2 < strlen(program)) {
            int opcode = program[IP * 2] - '0';
            int operand = program[IP * 2 + 2] - '0';

            switch(opcode) {
                case 0: 
                    adv(operand);
                    break;
                case 1:
                    bxl(operand);
                    break;
                case 2:
                    bst(operand);
                    break;
                case 3:
                    jnz(operand);
                    break;
                case 4:
                    bxc(operand);
                    break;
                case 5:
                    if (comma) {
                        strcat(output, ",");
                    } 
                    sprintf(number, "%lld", out(operand));
                    strcat(output, number);
                    comma = 1;
                    break;
                case 6:
                    bdv(operand);
                    break;
                case 7:
                    cdv(operand);
                    break;
                default:
                    printf("Incorrect opcode\n");
                    break;
            }
        }

        if (position == 15 && output[position * 2] != '0') {
            a += pow(8, position); 
            continue;
        } else if (position == 14 && output[position * 2] != '3') {
            a += pow(8, position); 
            continue;
        } else if (position == 13 && output[position * 2] != '5') {
            a += pow(8, position); 
            continue;
        } else if (position == 12 && output[position * 2] != '5') {
            a += pow(8, position); 
            continue;
        } else if (position == 11 && output[position * 2] != '3') {
            a += pow(8, position); 
            continue;
        } else if (position == 10 && output[position * 2] != '4') {
            a += pow(8, position); 
            continue;
        } else if (position == 9 && output[position * 2] != '3') {
            a += pow(8, position); 
            continue;
        } else if (position == 8 && output[position * 2] != '0') {
            a += pow(8, position);
        } else if (position == 7 && output[position * 2] != '5') {
            a += pow(8, position); 
        } else if (position == 6 && output[position * 2] != '1') {
            a += pow(8, position); 
        } else if (position == 5 && output[position * 2] != '5') {
            a += pow(8, position); 
        } else if (position == 4 && output[position * 2] != '7') {
            a += pow(8, position); 
        } else if (position == 3 && output[position * 2] != '1') {
            a += pow(8, position); 
        } else if (position == 2 && output[position * 2] != '1') {
            a += pow(8, position); 
        } else if (position == 1 && output[position * 2] != '4') {
            a += pow(8, position); 
        } else if (position == 0 && output[position * 2] != '2') {
            a += pow(8, position);
        } else {
            position--;
        }

        if (strcmp(program, output) == 0) { 
            break;
        }
        a++;
    }
    printf("A: %lld\n", a);
    free(output);
}

long long int combo(int operand) {
    if (operand >= 0 && operand <= 3)   return operand;
    if (operand == 4)                   return A;
    if (operand == 5)                   return B;
    if (operand == 6)                   return C;

    return 0;
}

void adv(int operand) {
   A = A / pow(2, combo(operand)); 
   IP += 2;
}

void bxl(int operand) {
    B = B ^ operand;
    IP += 2;
}

void bst(int operand) {
    B = combo(operand) % 8;
    IP += 2;
}

void jnz(int operand) {
    if (A == 0) IP += 2;
    else        IP = operand;
}

void bxc(int operand) {
    B = B ^ C;
    IP += 2;
}

long long int out(int operand) {
    IP += 2;
    return combo(operand) % 8;
}

void bdv(int operand) {
    B = A / pow(2, combo(operand)); 
    IP += 2;
}

void cdv(int operand) {
    C = A / pow(2, combo(operand)); 
    IP += 2;
}
