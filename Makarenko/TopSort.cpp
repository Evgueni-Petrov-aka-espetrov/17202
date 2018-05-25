#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

int check(int n, int m, FILE* output) {
	
	if (n < 0 || n > 1000) {
		fprintf_s(output, "bad number of vertices");
		return 0;
	}

	if (m < 0 || m > (n * (n + 1) / 2)) {
		fprintf_s(output, "bad number of edges");
		return 0;
	}
	return 1;
}
int reading(int n, int m, FILE* output ,FILE* input, vector<int> *vertex) {
	if (feof(input)) {
		fprintf_s(output, "bad number of vertices");
		return 0;
	}
	if (check(n, m, output) == 0) {
		return 0;
	}

	for (int i = 0; i < m; i++) {
		int begin;
		int end;
		fscanf_s(input, "%d", &begin);
		fscanf_s(input, "%d", &end);
		if (feof(input) && i < m ) {
			fprintf_s(output, "bad number of lines");
			return 0;
		}
		if (begin < 1 || begin > n || end > n || end < 1) {
			fprintf_s(output, "bad vertex");
			return 0;
		}
		vertex[begin - 1].push_back(end - 1);
	}
	if (check(n, m, output) == 0) {
		return 0;
	}
	fclose(input);
	return 1;
	
}
int dfs(vector<int> *vertex, vector<int> *result, int *color, const int ver) {
	if (color[ver] == 1) {
		return 1;
	}
	if (color[ver] == 2) {
		return 0;
	}
	color[ver] = 1;
	for (int i = 0; i < vertex[ver].size(); i++) {
		if (vertex[ver][i] != -1) {
			if (dfs(vertex, result, color, vertex[ver][i])) {
				return 1;
			}
		}
	}
	result->push_back(ver + 1);
	color[ver] = 2;
	return 0;
}
void printing(vector<int> result,FILE* output) {
	for (int i = result.size() - 1; i >= 0; --i) {
		if (result[i])
			fprintf_s(output, "%d ", result[i]);
	}
	return;
}
void topSort(int n, int m, FILE* input, FILE* output) {
	if (feof(input)) {
		fprintf_s(output, "bad number of lines");
		return;
	}
	vector<vector<int>> vertex(n);
	vector<int> result(n, 0);
	vector<int> color(n, 0);
	if (!reading(n, m, output, input ,&vertex[0])) {
		return;
	}
	for (int i = 0; i < n; i++) {
		if (dfs(&vertex[0], &result, &color[0], i) == 1) {
			fprintf_s(output, "impossible to sort");
			return;
		}
	}
	printing(result, output);
}
int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	int n;
	int m;
	fscanf_s(input, "%d%d", &n, &m);
	topSort(n, m, input, output);
	return 0;
}
