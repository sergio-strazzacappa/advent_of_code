#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define INPUT_FILE "input.txt"
#define LINE_LEN 20
#define MAX_BOXES 1000
#define MAX_CONNECTIONS 1000
#define MAX_CIRCUITS 1000
#define MAX_GROUP 1000

typedef struct Box {
    int id;
    int x;
    int y;
    int z;
} Box;

typedef struct Connection {
    int id1;
    int id2;
} Connection;

typedef struct Circuit {
    int connected_boxes[MAX_GROUP];
    int size;
} Circuit;

typedef struct Distance {
    int id1;
    int id2;
    double distance;
} Distance;

Box boxes[MAX_BOXES];
int boxes_size = 0;

Connection connections[MAX_CONNECTIONS];
int connections_size = 0;

Circuit circuits[MAX_CIRCUITS];
int circuits_size = 0;

void solve();
double distance(Box p, Box q);
bool is_connected(int id1, int id2);
int get_circuit(int id);
int cmp(const void *a, const void *b);
void print_boxes();
void print_connections();
void print_circuits();

int main() {
    FILE *fp;
    char line[LINE_LEN];
    

    if ((fp = fopen(INPUT_FILE, "r")) == NULL) {
        printf("[ERROR] Can't open file %s\n", INPUT_FILE);
        return 1;
    }

    int id = 0;
    while (fgets(line, LINE_LEN, fp)) {
        // read each line
        char *x = strtok(line, ",\n");
        char *y = strtok(NULL, ",\n");
        char *z = strtok(NULL, ",\n");

        Box b = { id, atoi(x), atoi(y), atoi(z) };
        boxes[boxes_size++] = b;
        id++;
    }

    solve();
}

void solve() {
    int iteration = 0;
    Distance distances[500000];
    int distances_size = 0;

    for (int i = 0; i < boxes_size; i++) {
        for (int j = i + 1; j < boxes_size; j++) {
            double d = distance(boxes[i], boxes[j]);
            distances[distances_size++] = (Distance){ boxes[i].id, boxes[j].id, d };
        }
    }

    qsort(distances, distances_size, sizeof(Distance), cmp);

    while (circuits[0].size < boxes_size ) {
        printf("[DEBUG] Iteration %d - circuit size %d\n", iteration, circuits_size);

        int id1 = distances[iteration].id1;
        int id2 = distances[iteration].id2;

        if (is_connected(id1, id2)) {
            continue;
        }

        // add the connection
        Connection c = { id1, id2 };
        connections[connections_size++] = c;

        iteration++;
        
        // add the circuit
        int index1 = get_circuit(id1);
        int index2 = get_circuit(id2);

        if (index1 != -1 && index2 == -1) {
            // index 1 found, but not index 2
            circuits[index1].connected_boxes[circuits[index1].size++] = id2;
            continue;
        }
        if (index1 == -1 && index2 != -1) {
            // index 2 found, but not index 1
            circuits[index2].connected_boxes[circuits[index2].size++] = id1;
            continue;
        }
        if (index1 != index2) {
            // both indexes were found, merge the circuits
            for (int i = 0; i < circuits[index2].size; i++) {
                circuits[index1].connected_boxes[circuits[index1].size++] =
                    circuits[index2].connected_boxes[i];
            }
            for (int i = index2; i < circuits_size - 1; i++) {
                circuits[i] = circuits[i + 1];
            }
            circuits_size--;
            continue;
        }
        if (index1 == -1 && index2 == -1) {
            // neither box were found
            Circuit c = { {id1, id2}, 2};
            circuits[circuits_size++] = c; 
            continue;
        }
    }

    print_connections();
    print_circuits();

    int id1 = connections[connections_size - 1].id1;
    int id2 = connections[connections_size - 1].id2;
    long x1, x2;

    for (int i = 0; i < boxes_size; i++) {
        if (boxes[i].id == id1) {
            x1 = boxes[i].x;
        }
        if (boxes[i].id == id2) {
            x2 = boxes[i].x;
        }
    }

    printf("[SOLUTION] %ld\n", x1 * x2);
}

// return the euclidean distance between two boxes
double distance(Box p, Box q) {
    return (sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2) + pow(p.z - q.z, 2))); 
}

// return true if two boxes are connected
bool is_connected(int id1, int id2) {
    for (int i = 0; i < connections_size; i++) {
        if (connections[i].id1 == id1 && connections[i].id2 == id2) {
            return true;
        }
    }
    return false;
}

// return the circuit's index of id, or -1 if not found
int get_circuit(int id) {
    for (int i = 0; i < circuits_size; i++) {
        Circuit c = circuits[i];
        for (int j = 0; j < c.size; j++) {
            if (id == c.connected_boxes[j]) {
                return i;
            }
        }
    }
    return -1;
}

int cmp(const void *a, const void *b) {
    return ((Distance *)a)->distance - ((Distance *)b)->distance;
}

void print_boxes() {
    printf("[DEBUG] -----------------------------------------------\n");
    for (int i = 0; i < boxes_size; i++) {
        printf("[DEBUG] %d=(%d, %d, %d)\n",
            boxes[i].id, boxes[i].x, boxes[i].y, boxes[i].z);
    }
    printf("[DEBUG] -----------------------------------------------\n");
}

void print_connections() {
    printf("[DEBUG] CONNECTIONS:%d\n", connections_size);
    for (int i = 0; i < connections_size; i++) {
        printf("[DEBUG] %d -> %d\n", connections[i].id1, connections[i].id2);
    }
    printf("\n");
}

void print_circuits() {
    printf("[DEBUG] CIRCUITS:%d\n", circuits_size);
    for (int i = 0; i < circuits_size; i++) {
        Circuit c = circuits[i];
        printf("[DEBUG] {");
        for (int j = 0; j < c.size; j++) {
            printf(" %d ", c.connected_boxes[j]);
        }
        printf("}\n");
    }
    printf("\n");
}
