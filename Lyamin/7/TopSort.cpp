#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;

int checking(int N, int M) {
	if (N < 0 || N > 1000) {
		cout << "bad number of vertices";
		return 0;
	}

	if (M < 0 || M >(N*(N + 1) / 2)) {
		cout << "bad number of edges";
		return 0;
	}
	else {
		return 1;
	}
}

int reading(int N, int M, vector<int> *vertex, FILE* stream) {
	if (feof(stream)) {
		cout << "bad number of lines";
		return 0;
	}
	if (checking(N, M) == 0) {
		return 0;
	}

	for (int i = 0; i < M; ++i) {
		int begin, end;
		fscanf(stream, "%d", &begin);
		fscanf(stream, "%d", &end);
		if (feof(stream) && i < M) {
			cout << "bad number of lines";
			return 0;
		}
		if (begin < 1 || begin > N || end > N || end < 1) {
			cout << "bad vertex";
			return 0;
		}
		vertex[begin - 1].push_back(end - 1);
	}
	if (checking(N, M) == 0) {
		return 0;
	}
	fclose(stream);
	return 1;
}

void printing(vector<int> result) {
	for (int i = result.size() - 1; i >= 0; --i) {
		if (!result[i] == 0)
		cout << result[i] << ' ';
	}
	return;
}

int dfs(const int ver, int *color, vector<int> *vertex, vector<int> *result) {
	if (color[ver] == 1) { return 1; };
	if (color[ver] == 2) { return 0; };
	color[ver] = 1;
	for (int i = 0; i < vertex[ver].size(); ++i) {
		if (vertex[ver][i] != -1) {
			if (dfs(vertex[ver][i], color, vertex, result)) {
				return 1;
			}
		}
	}
	result->push_back(ver + 1);
	color[ver] = 2;
	return 0;
}

void topSort(int N, int M, FILE* stream) {
	if (feof(stream)) {
		cout << "bad number of lines";
		return;
	}
	vector<vector<int>> vertex(N);
	vector<int> result(N, 0);
	vector<int> color(N, 0);

	if (!reading(N, M, &vertex[0], stream)) {
		return;
	}

	for (int i = 0; i < N; ++i) {
		if (dfs(i, &color[0], &vertex[0], &result) == 1) {
			cout << "impossible to sort";
			return;
		}
	}
	printing(result);
}

int main() {
	FILE* stream = fopen("in.txt", "r");
	int N, M;
	fscanf(stream, "%d %d", &N, &M);
	topSort(N, M, stream);
	return 0;
}
