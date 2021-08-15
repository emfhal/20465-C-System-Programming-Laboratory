#include <stdio.h>

#define N 11

enum bool {
    FALSE, TRUE
};
typedef int adjmat[N][N];

void printADJ(adjmat);

void getADJ(adjmat);

int path(adjmat, int, int);

int main() {
    adjmat mat;
    printADJ(mat);
    getADJ(mat);
    return 0;
}

/* function to print matrix on the user screen */
void printADJ(adjmat mat) {
    int i = 0, j = 0;
    printf("Please enter %d numbers for adjacency matrix, representing a tree (Father and son) :\n", N * N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            scanf("%d", &mat[i][j]);
            printf("%d ", mat[i][j]);
        }
        fputc('\n', stdout);
    }
}

/* function to print matrix on the user screen */
void getADJ(adjmat mat) {
    int u, v;
    printf("Please type 2 number (the first number is the node father and the second is the child) \n for cheking if there is a path between the two nodes of them:\n");
    while (scanf("%d %d", &u, &v) == 2 && (u != -1 || v != -1)) {
        printf("There is%s a path between nodes %d (father) and %d(son) \n", (path(mat, u, v)) ? "" : "n\'t", u, v);
        printf("Please type 2 number (the first number is the node father and the second is the child) \n for cheking if there is a path between the two nodes of them:\n");
    }
}

/* function to find if the path exsit between (u,v) the nodes of the graph*/
int path(adjmat mat, int u, int v) {
    int i = 0;
    if (u <= N || v >= N || u < 0 || v < 0)
        return FALSE;
    if (u == v)
        return TRUE;
    for (; i < N; i++) {
        if (u != i && mat[u][i] && path(mat, i, v))
            return TRUE;
    }
    return FALSE;
}

