#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE  "input.txt"
#define LINE_LEN    10
#define NET_SIZE    20000

typedef struct link {
    char *pc1;
    char *pc2;
} Link;

typedef struct party {
    char *pc1;
    char *pc2;
    char* pc3;
} Party;

void part1(Link net[], int net_index);
void part2(Link net[], int net_index);
void solve(char *nodes[], int nodes_index, Link net[], int net_index,
    int current, char *current_group[], int *current_group_index,
    char *max_group[], int *max_group_size);
int equal_party(Party p, char *pc1, char *pc2, char *pc3);
int comp(const void *a, const void *b);
void print_net(Link net[], int net_index);

int main() {
    FILE *fp;
    char line[LINE_LEN], *pc1, *pc2;
    Link net[NET_SIZE];
    int net_index;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    net_index = 0;
    while (fgets(line, LINE_LEN,fp)) {
        // saves every link in both directions
        Link l1, l2;

        pc1 = strtok(line, "-\n");
        pc2 = strtok(NULL, "\n");
        
        l1.pc1 = (char *)malloc(sizeof(char) * 2);
        l1.pc2 = (char *)malloc(sizeof(char) * 2);
        strcpy(l1.pc1, pc1);
        strcpy(l1.pc2, pc2);
        net[net_index++] = l1;

        l2.pc1 = (char *)malloc(sizeof(char) * 2);
        l2.pc2 = (char *)malloc(sizeof(char) * 2);
        strcpy(l2.pc1, pc2);
        strcpy(l2.pc2, pc1);
        net[net_index++] = l2;
    }

    part1(net, net_index);
    part2(net, net_index);

    fclose(fp);
    return 0;
}

void part1(Link net[], int net_index) {
    Party lan[NET_SIZE];
    int groups, found, lan_index;

    groups = 0;
    lan_index = 0;
    // for every link
    for (int i = 0; i < net_index; i += 2) {
        char *pc1, *pc2, *pc3, *match[NET_SIZE];
        int match_index;

        pc1 = (char *)malloc(sizeof(char) * 2);
        strcpy(pc1, net[i].pc1);
        pc2 = (char *)malloc(sizeof(char) * 2);
        strcpy(pc2, net[i].pc2);
        match_index = 0;

        // search a third pc
        for (int j = 0; j < net_index; j++) {
            if (i == j) continue;

            found = 0;
            pc3 = NULL;
            if ((strcmp(pc1, net[j].pc1) == 0 && strcmp(pc2, net[j].pc2) != 0) || 
                (strcmp(pc2, net[j].pc1) == 0 && strcmp(pc1, net[j].pc2) != 0)) {
                pc3 = (char *)malloc(sizeof(char) * 2);
                strcpy(pc3, net[j].pc2);
            }
            // a pc was found
            if (pc3 != NULL) {
                // search if the pc was already saved
                for (int k = 0; k < match_index; k++) {
                    if (strcmp(match[k], pc3) == 0) {
                        found = 1;
                        break;
                    }
                }
                // if not, save it
                if (!found) {
                    match[match_index] = (char *)malloc(sizeof(char) * 2);
                    strcpy(match[match_index++], pc3);
                }
                // if it is, saved it as a lan if it is not there
                if (found) {
                    found = 0;
                    for (int k = 0; k < lan_index; k++) {
                        if (equal_party(lan[k], pc1, pc2, pc3) == 1) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        Party p;
                        p.pc1 = (char *)malloc(sizeof(char) * 2);
                        p.pc2 = (char *)malloc(sizeof(char) * 2);
                        p.pc3 = (char *)malloc(sizeof(char) * 2);
                        strcpy(p.pc1, pc1);
                        strcpy(p.pc2, pc2);
                        strcpy(p.pc3, pc3);
                        lan[lan_index++] = p;
                    }
                }
            }
        }
    }

    for (int i = 0; i < lan_index; i++) {
        if (lan[i].pc1[0] == 't' || lan[i].pc2[0] == 't' || lan[i].pc3[0] == 't') {
            groups++;
        }
    }
    printf("The number of groups is %d\n", groups);
}

void part2(Link net[], int net_index) {
    char *nodes[NET_SIZE], *max_group[NET_SIZE], *current_group[NET_SIZE];
    int nodes_index, found, max_group_index, current_group_index;

    // create an array with the nodes
    nodes_index = 0;
    for (int i = 0; i < net_index; i += 2) {
        found = 0;
        for (int j = 0; j < nodes_index; j++) {
            if (strcmp(nodes[j], net[i].pc1) == 0) {
                found = 1;
                break;
            }
        } 
        if (!found) {
            nodes[nodes_index] = (char *)malloc(sizeof(char) * 2);
            strcpy(nodes[nodes_index++], net[i].pc1);
        }
    }

    max_group_index = 0;

    for (int i = 0; i < nodes_index; i++) {
        current_group_index = 0;

        // finds the max set, starting from nodes[i]
        solve(nodes, nodes_index, net, net_index, i, current_group,
            &current_group_index, max_group, &max_group_index);

        // check if the set is the greater so far
        if (current_group_index > max_group_index) {
            for (int j = 0; j < current_group_index; j++) {
                max_group[j] = (char *)malloc(sizeof(char) * 2);
                strcpy(max_group[j], current_group[j]);
                max_group_index = current_group_index;
            }
        }
    }

    // sort the max group lexicographic
    qsort(max_group, max_group_index / sizeof(char), sizeof(max_group[0]), comp);

    printf("Solution for part 2: ");
    for (int i = 0; i < max_group_index; i++) {
        printf("%s", max_group[i]);
        if (i < max_group_index - 1) {
            printf(",");
        }
    }
    printf("\n");
}

void solve(char *nodes[], int nodes_index, Link net[], int net_index,
    int current, char *current_group[], int *current_group_index,
    char *max_group[], int *max_group_size) {
    int in;
    // add the current node to the set
    current_group[(*current_group_index)++] = nodes[current];


    // check every node greater than the current
    // if it is connected to every other node in the set
    // call solve recurively, if not skip it
    for (int i = current + 1; i < nodes_index; i++) {
        for (int j = 0; j < *current_group_index; j++) {
            in = 0;
            for (int k = 0; k < net_index; k++) {
                if (strcmp(nodes[i], net[k].pc1) == 0 && strcmp(current_group[j], net[k].pc2) == 0) {
                    in = 1;
                    break;
                }
            }
            if (!in)
                break;
        }
        if (in)
            solve(nodes, nodes_index, net, net_index, i, current_group, current_group_index, max_group, max_group_size);
    }
}

int equal_party(Party p, char *pc1, char *pc2, char *pc3) {
    int pc1_found, pc2_found, pc3_found;
    pc1_found = 0;
    pc2_found = 0;
    pc3_found = 0;

    if (strcmp(p.pc1, pc1) == 0 || strcmp(p.pc1, pc2) == 0 || strcmp(p.pc1, pc3) == 0)
        pc1_found = 1;
    if (strcmp(p.pc2, pc1) == 0 || strcmp(p.pc2, pc2) == 0 || strcmp(p.pc2, pc3) == 0)
        pc2_found = 1;
    if (strcmp(p.pc3, pc1) == 0 || strcmp(p.pc3, pc2) == 0 || strcmp(p.pc3, pc3) == 0)
        pc3_found = 1;

    return pc1_found && pc2_found && pc3_found;
}

int comp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_net(Link net[], int net_index) {
    for (int i = 0; i < net_index; i++) {
        printf("%s <-> %s\n", net[i].pc1, net[i].pc2);
    }
}
