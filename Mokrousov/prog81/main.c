#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#define TRUE 1

typedef struct TVertexInfo {
	int is_passed;
	int min_dist;
	int from_min_dist;	
} TVertexInfo;

void print_carcas(FILE *fin, FILE *fout, int **adjacency_matrix, int n)
{
	TVertexInfo *vertexes = malloc(n*sizeof(TVertexInfo));
	for (int i = 0; i < n; i++)
	{
		vertexes[i].is_passed = 0;
		vertexes[i].from_min_dist = -1;
		vertexes[i].min_dist = -1;
	}
	for (int i = 0; i < n; i++)
	{
		int min_dist = -1;
		int min_vertex = -1;
		for (int j = 0; j < n; j++)
			if ((!vertexes[j].is_passed) && ((min_dist == -1) || ((min_dist != -1) && (vertexes[j].min_dist < min_dist))))
			{
				min_dist = vertexes[j].min_dist;
				min_vertex = j;
			}
		if (min_dist != -1)
		{
			fprintf(fout, "%d %d\n", vertexes[min_vertex].from_min_dist+1, min_vertex+1);
		}
		vertexes[min_vertex].is_passed = 1;
		for (int j = 0; j < n; j++)
		if ((!vertexes[j].is_passed) && (adjacency_matrix[min_vertex][j] != -1) && ((adjacency_matrix[min_vertex][j] < vertexes[j].min_dist) || (vertexes[j].min_dist==-1)))
			{
				vertexes[j].min_dist = adjacency_matrix[min_vertex][j];
				vertexes[j].from_min_dist = min_vertex;
			}
	}
}
void error(FILE *fin, FILE *fout, char *message) { //function for outputing errors
	fprintf(fout, message);
	fclose(fin);
	fclose(fout);
	exit(0);
}

void read_adjacency_matrix(FILE *fin, FILE *fout, int n, int m, int **adjacency_matrix) {
	int i, j;
	for (i = 0; i < n; i++)
	for (j = 0; j < n; j++)
		adjacency_matrix[i][j] = -1;
	for (i = 0; i < m; i++) {
		int vertex1, vertex2;
		long long length;
		//scanning & checking all vertexes with lengths between them
		int temp = fscanf(fin, "%d %d %I64d\n", &vertex1, &vertex2, &length);
		if (temp == EOF)
			error(fin, fout, "bad number of lines");
		if (length > INT_MAX || length < 0)
			error(fin, fout, "bad length");
		if (vertex1 < 1 || vertex1 > n)
			error(fin, fout, "bad vertex");
		if (vertex2 < 1 || vertex2 > n)
			error(fin, fout, "bad vertex");
		vertex1--; //decrementing for using, starting with 0
		vertex2--;
		adjacency_matrix[vertex1][vertex2] = length;
		adjacency_matrix[vertex2][vertex1] = length;
	}
}

int check_graph(int **adjacency_matrix, int n)
{
	if (n == 0)
		return 0;
	int queue[5000];
	int flags[5000] = { 0 };
	int head = 0;
	int tail = 0;
	queue[tail++] = 0;

	while (head != tail) //until we have checked all elements of queue
	{
		int cur_vertex = queue[head++];
		flags[cur_vertex] = 1; //as visited
		int i;
		for (i = 0; i < n; i++)
		if ((adjacency_matrix[cur_vertex][i] != -1) && (flags[i] == 0)) //if vertix wasn't visited and there's a path
		{
			queue[tail++] = i; //putting at the end of queue
			flags[i] = 1; //as visited
		}
	}

	int i;
	for (i = 0; i < n; i++)
	if (flags[i] == 0) //if one of the vertixes is unconnected with others
		return 0; //we have no frame
	return 1;
}

int main() {

	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");

	int n, m;
	int start, end, length;
	int **adjacency_matrix;
	int *flags;
	int temp = fscanf(fin, "%d\n", &n); //scanning and checking n
	if (temp == EOF)
		error(fin, fout, "bad number of lines");
	if (n < 0 || n > 5000)
		error(fin, fout, "bad number of vertices");

	temp = fscanf(fin, "%d\n", &m); //scanning and checking m
	if (temp == EOF)
		error(fin, fout, "bad number of lines");
	if (m < 0 || m > n * (n - 1) / 2)
		error(fin, fout, "bad number of edges");

	adjacency_matrix = (int**)malloc(sizeof(int*)* n);
	for (int i = 0; i < n; i++)
		adjacency_matrix[i] = (int*)malloc(sizeof(int)*n);
	read_adjacency_matrix(fin, fout, n, m, adjacency_matrix);

	if (!check_graph(adjacency_matrix, n)) {
		error(fin, fout, "no spanning tree");
		free(adjacency_matrix);
		return 0;
	}

	print_carcas(fin, fout, adjacency_matrix, n);
}