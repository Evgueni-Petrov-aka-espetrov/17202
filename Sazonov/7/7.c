#include <stdio.h>
#include <stdlib.h>

/* очень прожорливый на память (полная матрица смежности == ошибка) и крашится на одном из последних тестов если компилировать в x64 */

void error(int errorCode) {
    FILE *output_file = fopen("out.txt", "w");
    switch (errorCode) {
        case 0:
            fprintf(output_file, "bad number of vertices");
            break;
        case 1:
            fprintf(output_file, "bad number of edges");
            break;
        case 2:
            fprintf(output_file, "bad vertex");
            break;
        case 3:
            fprintf(output_file, "bad number of lines");
            break;
        case 4:
            fprintf(output_file, "impossible to sort");
            break;
    }
    fclose(output_file);
    exit(0);
}

void ans_push(int value, int *ans) {
    for (int i = 0; 1; ++i)
        if (ans[i] == 0) {
            ans[i] = value;
            return;
        }
}

void DFS(int i, int *edge, int points, int *flag, int *ans) {
    flag[i] = 1;
    for (int j = 0; j < points; ++j) {
        if (edge[i * points + j] == 1) {
            if (flag[j] == 0)
                DFS(j, edge, points, flag, ans);
            if (flag[j] == 1)
                error(4);
        }
    }
    ans_push(i + 1, ans);
    flag[i] = 2;
}

void topologicalSort(int *edge, int points, int *flag, int *ans) {
    for (int i = 0; i < points * points; ++i) {
        if (edge[i] == 1)
            if (flag[i] == 0)
                DFS(i / points, edge, points, flag, ans);
    }
}

int main() {
    FILE *input_file = fopen("in.txt", "r");
    int n = -42, m = -42;
    fscanf(input_file, "%d\n%d\n", &n, &m);
    if (n == -42 || m == -42) {
        fclose(input_file);
        error(3);
    }
    if (n < 0 || n > 1000) {
        fclose(input_file);
        error(0);
    }
    if (m < 0 || m > n * (n + 1) / 2) {
        fclose(input_file);
        error(1);
    }
    int *edge = (int *) calloc((size_t) n * n, sizeof(int));
    int i;
    for (i = 0; !feof(input_file); ++i) {
        int begin, end;
        fscanf(input_file, "%d %d\n", &begin, &end);
        if (begin < 1 || begin > n || end < 1 || end > n) {
            fclose(input_file);
            error(2);
        }
        edge[(--begin) * n + (--end)] = 1;
    }
    fclose(input_file);
    if (i < m) {
        error(3);
    }
    int *ans = (int *) calloc((size_t) n, sizeof(int));
    int *flag = (int *) calloc((size_t) n, sizeof(int));
    topologicalSort(edge, n, flag, ans);
    free(edge);
    int *reserve = (int *) calloc((size_t) n, sizeof(int));
    FILE *output_file = fopen("out.txt", "w");
    for (i = n - 1; i > -1; --i)
        if (ans[i] != 0) {
            fprintf(output_file, "%d ", ans[i]);
            reserve[ans[i] - 1] = -1;
        }
    for (i = 0; i < n; ++i)
        if (reserve[i] != -1) fprintf(output_file, "%d ", i + 1);
    free(ans);
    free(flag);
    free(reserve);
    fclose(output_file);
    return 0;
}
