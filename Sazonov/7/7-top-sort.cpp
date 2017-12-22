#include <iostream>
#include <vector>

/* use with -m 1600 */

using namespace std;

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

void DFS(int i, vector<vector<char>> *edge, int points, char *flag, vector<int> *ans) {
    flag[i] = 1;
    for (int j = 0; j < points; ++j) {
        if ((*edge)[i][j] == 1) {
            if (flag[j] == 0)
                DFS(j, edge, points, flag, ans);
            if (flag[j] == 1)
                error(4);
        }
    }
    (*ans).push_back(i);
    flag[i] = 2;
}

void topologicalSort(vector<vector<char>> *edge, int points, char *flag, vector<int> *ans) {
    for (int i = 0; i < points; ++i)
        for (int j = 0; j < points; ++j)
            if ((*edge)[i][j] == 1)
                if (flag[i] == 0)
                    DFS(i, edge, points, flag, ans);

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
    vector<vector<char>> edge(n);
    for (int i = 0; i < n; ++i) {
        edge[i].resize(n);
    }
    int i;
    for (i = 0; !feof(input_file); ++i) {
        int begin, end;
        fscanf(input_file, "%d %d\n", &begin, &end);
        if (begin < 1 || begin > n || end < 1 || end > n) {
            fclose(input_file);
            error(2);
        }
        --begin; --end;
        edge[begin][end] = 1;
    }
    fclose(input_file);
    if (i < m) {
        error(3);
    }
    vector<int> ans;
    char *flag = (char *)calloc((size_t)n, sizeof(char));
    topologicalSort(&edge, n, flag, &ans);
    char *reserve = (char *)calloc((size_t)n, sizeof(char));
    FILE *output_file = fopen("out.txt", "w");
    for (int i = ans.size() - 1; i > -1; --i) {
        fprintf(output_file, "%d ", ans[i] + 1);
        reserve[ans[i]] = -1;
    }
    if (ans.size() < n) {
        for (i = 0; i < n; ++i) {
            if (reserve[i] != -1)
                fprintf(output_file, "%d ", i+1);
        }
    }
    free(flag);
    free(reserve);
    fclose(output_file);
    return 0;
}
