#define _CRT_SECURE_NO_WARNINGS
#include <memory>
#include <iostream>
#include <vector>
using namespace std;
FILE *openInputStream() {//особая магия
	FILE *stream;
	return stream = fopen("in.txt", "r");
}
bool isValid(const int numOfVer, const int numOfEdges) {
	if (numOfVer < 0 || numOfVer > 1000) {
		printf("bad number of vertices");
		return false;
	}
	if (numOfEdges < 0 || numOfEdges >(numOfVer*(numOfVer + 1)) / 2) {
		printf("bad number of edges");
		return false;
	}
	return true;
}
bool readGraph(vector<int> *graph, const int numberOfVertex, const int numberOfEdges, FILE *stream) {
	if (feof(stream)) {
		printf("bad number of lines");
		return false;
	}
	for (int i = 0; i < numberOfEdges; ++i) {
		int v1 = 0, v2 = 0;
		fscanf(stream, "%d", &v1);
		fscanf(stream, "%d", &v2);
		if (feof(stream) && i < numberOfEdges) {
			printf("bad number of lines");
			return false;
		}
		if (v1 < 1 || v1 > numberOfVertex || v2 < 1 || v2 > numberOfVertex) {
			printf("bad vertex");
			return false;
		}
		graph[v1 - 1].push_back(v2 - 1);
	}
	return true;
}
bool deepGo(const int ver, int *color, vector<int> *graph, vector<int> * result) {
	if (color[ver] == 1) return true;
	if (color[ver] == 2) return false;
	color[ver] = 1;
	for (unsigned int i = 0; i < graph[ver].size(); ++i) {
		if (deepGo(graph[ver][i], &color[0], &graph[0], &result[0])) {
			return true;
		}
	}
	result->push_back(ver + 1);
	color[ver] = 2;
	return false;
}
void printRes(const int *result, int size) {
	for (int i = size - 1; i >= 0; --i) {
		if (result[i] != 0)
			cout << result[i] << " ";
	}
}
void topSort(const int numOfVer, const int numbOfEdges, FILE *stream) {
	vector <vector<int>> graph(numOfVer);
	vector <int> result(numOfVer);
	vector <int> color(numOfVer);
	for (int i = 0; i < numOfVer; ++i) {
		color[i] = 0;
	}

	if (!readGraph(&graph[0], numOfVer, numbOfEdges, stream)) {
		return;
	}
	bool isCycled = false;
	for (int i = 0; i < numOfVer; ++i) {
		isCycled = deepGo(i, &color[0], &graph[0], &result);
		if (isCycled) {
			printf("impossible to sort");
			return;
		}
	}

	printRes(&result[0], result.size());
}
void main() {
	FILE *stream = openInputStream();
	int numOfVer = 0, numOfEdges = 0;
	fscanf(stream, "%d %d", &numOfVer, &numOfEdges);
	if (!isValid(numOfVer, numOfEdges)) {
		return;
	}
	topSort(numOfVer, numOfEdges, stream);
}
