#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"
#define N_FILES 10000
#define LINE_LEN 20000
#define TOTAL_SIZE 90000

typedef struct file {
    int id;
    int size;
    int blocks[10]; // block position
} file;

void part1(file files[N_FILES], int f_index, int fs[TOTAL_SIZE], int fs_index);
void part2(file files[N_FILES], int f_index, int fs[TOTAL_SIZE], int fs_index);
long long int checksum(file files[N_FILES], int n);

void print_fs(int fs[TOTAL_SIZE], int fs_index);
void print_files(file files[N_FILES], int f_index);

int main() {
    FILE *fp;
    char line[LINE_LEN];
    int fs[TOTAL_SIZE], fs2[TOTAL_SIZE], index, fs_index, f_index, pos;
    file files[N_FILES], files2[N_FILES];

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open %s\n", INPUT_FILE);
        return 1;
    }

    index = 0;
    fs_index = 0;
    pos = 0;
    f_index = 0;
    while (fgets(line, LINE_LEN, fp)) {
        for (int i = 0; i < strlen(line) && line[i] != '\n'; i++) {
            int val = line[i] - '0';
            if (i % 2 == 0) {
                // read a file
                file f;
                f.id = i / 2;
                f.size = val;

                for (int j = 0; j < f.size; j++) {
                    f.blocks[j] = index;
                    index++;
                }

                files[f_index] = f;
                files2[f_index] = f;
                f_index++;
            } else {
                // read free block
                for (int j = 0; j < val; j++) {
                    fs[fs_index] = pos + j;
                    fs2[fs_index] = pos + j;
                    fs_index++;
                    index++;
                }
            }
            pos += val;
        }
    }

    part1(files, f_index, fs, fs_index);
    part2(files2, f_index, fs2, fs_index);

    fclose(fp);
    return 0;
}

void part1(file files[N_FILES], int f_index, int fs[TOTAL_SIZE], int fs_index) {
    for (int i = f_index - 1; i >= 0; i--) {
        file *f = &files[i];
        for (int j = f->size - 1; j >= 0; j--) {
            // every block of a file
            for (int k = 0; k < fs_index; k++) {
                // search a free block
                if (fs[k] != -1 && fs[k] < f->blocks[j]) {
                    f->blocks[j] = fs[k];
                    fs[k] = -1; 
                    break;
                }
            }    
        }
    }
    printf("The checksum for part 1 is: %lld\n", checksum(files, f_index));
}

void part2(file files[N_FILES], int f_index, int fs[TOTAL_SIZE], int fs_index) {
    for (int i = f_index - 1; i >= 0; i--) {
        file *f = &files[i];

        if (f->size == 0) {
            // size is 0, go to the next file
            continue;
        }

        int block_size = 0;
        int prev_block = -1;

        for (int j = 0; j < fs_index; j++) {
            // search a free blocks
            if (fs[j] != -1) {
                // there is a free block
                if (prev_block == -1 || fs[j] == prev_block + 1) {
                    if (f->blocks[0] < fs[j]) {
                        // first block of the file is lower than first free
                        // block, no need to realocate
                        break;
                    }
                    block_size++;
                    prev_block = fs[j];
                    if (block_size == f->size) {
                        for (int k = 0; k < f->size; k++) {
                            f->blocks[f->size - k - 1] = fs[j - k];
                            fs[j - k] = -1;
                        }
                        break;
                    }
                } else {
                    prev_block = -1;
                    block_size = 0;
                    j--;
                }
            } else {
                // block was assigned to a file
                prev_block = -1;
                block_size = 0;
            }
        }
    }
    printf("The checksum for part 2 is: %lld\n", checksum(files, f_index));
}

long long int checksum(file files[N_FILES], int n) {
    long long int checksum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < files[i].size; j++) {
            checksum += files[i].id * files[i].blocks[j];
        }
    }
    return checksum;
}

void print_fs(int fs[TOTAL_SIZE], int fs_index) {
    printf("Free space: \n[ ");
    for (int i = 0; i < fs_index; i++) {
        printf("%d", fs[i]);
        if (i < fs_index - 1) printf(", ");
    }
    printf(" ]\n");
}

void print_files(file files[N_FILES], int f_index) {
    for (int i = 0; i < f_index; i++) {  
        printf("FILE: id=%d, size=%d \n", files[i].id, files[i].size);
        for (int j = 0; j < files[i].size; j++) {
            printf("b=%d ", files[i].blocks[j]);
        }
        printf("\n");
    }
}
