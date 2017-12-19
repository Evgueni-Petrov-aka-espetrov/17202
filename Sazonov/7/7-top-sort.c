#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int findNextDestination(int num, int lastChecked, int vert, const int edge[vert][vert]) {
    for (int i = ++lastChecked; i < vert; ++i)
        if (edge[num][i]) return i;
    return -1;

}

void clear(int *array, int vert) {
    for (int i = 0; i < vert; ++i) {
        array[i] = -1;
    }
}

void add(int *array, int value) {
    int i;
    for (i = 0; array[i] != -1; ++i);
    array[i] = value;
}

void DFS(int num, int vert, int *checked, const int **edge, int *ans) {
    checked[num] = 1;
    for (int i = 0; i < vert; ++i) {
        int destination = -1;
        while (1) {
            destination = findNextDestination(num, destination, vert, edge);
            if (destination == -1) break;
            if (!checked[destination])
                DFS(destination, vert, checked, edge, ans);
        }
    }
    add(ans, num);
}

void topological_sort(int vert, int *checked, const int edge[vert][vert], int *ans, int *loopCheck) {
    for (int i = 0; i < vert; ++i)
        checked[i] = 0;
    clear(ans, vert);
    for (int i = 0; i < vert; ++i) if (!checked[i]) DFS(i, vert, checked, edge, ans);
}

void error(int errorCode, FILE *output_file) {
    switch (errorCode) {
        case 0 :
            fprintf(output_file, "bad number if vertices");
        case 1 :
            fprintf(output_file, "bad number if edges");
        case 2 :
            fprintf(output_file, "bad vertex");
        case 3 :
            fprintf(output_file, "bad number of lines");
        case 4 :
            fprintf(output_file, "impossible to sort");
    }
    fclose(output_file);
    exit(0);
}


int main() {
    FILE *input_file = fopen("in.txt", "r"), *output_file = fopen("out.txt", "w");
    int n, m;
    fscanf(input_file, "%d\n%d\n", &n, &m);
    if (n < 0 || n > 1000) {
        fclose(input_file);
        error(0, output_file);
    }
    if (m < 0 || m > n * (n + 1) / 2) {
        fclose(input_file);
        error(1, output_file);
    }
    int edge[m][m];
    memset(edge, 0, m * m * sizeof(int));
    int i;
    for (i = 0; !feof(input_file); ++i) {
        int begin, end;
        if(begin < 1 || begin > n || end <1 || begin > n){
            fclose(input_file);
            error(2, output_file);
        }
        fscanf(input_file, "%d %d\n", &begin, &end);
        edge[--begin][--end] = 1;
    }
    fclose(input_file);
    if(i < m + 2){
        error(3, output_file);
    }
    int ans[m];
    memset(ans, -1, m * sizeof(int));
    int loopCheck[m];
    memset(loopCheck, 0, m * sizeof(int));
    int checked[m];
    memset(checked, 0, m * sizeof(int));
    topological_sort(n, &checked[0], edge, &ans[0], &loopCheck[0]);
    fprintf(output_file, "1 ");
    for (int k = m - 1; k > -1; --k)
        if (ans[k] != -1) fprintf(output_file, "%d ", ans[k] + 1);
    fclose(output_file);
    return 0;
}
