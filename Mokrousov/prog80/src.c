#include <stdio.h>
#include <malloc.h>
#include <limits.h>

struct TEdge //struct(type) for edges
{
	short vertex_1, vertex_2;
	int length;
};

#define TEdge struct TEdge

int comparing_func(TEdge *val1, TEdge *val2) //function for qsort
{
	return val1->length - val2->length;
}

void error(FILE *fin, FILE *fout, char *message) { //function for outputing errors
	fprintf(fout, message);
	fclose(fin);
	fclose(fout);
	exit(0);
}
//reading of matrix and filling the information about vertices
void read_adjacency_matrix(FILE *fin, FILE *fout, int n, int m, int *adjacency_matrix, TEdge *edges) {
	int i;
	for (i = 0; i < n * n; i++)
		adjacency_matrix[i] = -1;
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
		adjacency_matrix[vertex1 * n + vertex2] = length;
		adjacency_matrix[vertex2 * n + vertex1] = length;
		edges[i].vertex_1 = vertex1;
		edges[i].vertex_2 = vertex2;
		edges[i].length = length;
	}
}

int check_graph(int *adjacency_matrix, int n)
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
		for (int i = 0; i < n; i++)
		if ((adjacency_matrix[cur_vertex * n + i] != -1) && (flags[i] == 0)) //if vertix wasn't visited and there's a path
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

void print_edge(FILE *fout, int from, int to) //printing all edges, starting with 1 to n
{
	fprintf(fout, "%d %d\n", from + 1, to + 1);
}

int main() {

	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");

	int n, m;
	int start, end, length;
	int *adjacency_matrix;
	int temp = fscanf(fin, "%d\n", &n); //scanning and checking n
	if (temp == EOF)
		error(fin, fout, "bad number of lines");
	if (n < 0 || n > 5000)
		error(fin, fout, "bad number of vertices");

	temp = fscanf(fin, "%d\n", &m); //scanning and checking m
	if (temp == EOF)
		error(fin, fout, "bad number of lines");
	if (m < 0 || m > n*(n - 1) / 2)
		error(fin, fout, "bad number of edges");

	TEdge* edges = (TEdge*)malloc(m * sizeof(TEdge));
	adjacency_matrix = (int*)malloc(sizeof(int)* n * n);
	read_adjacency_matrix(fin, fout, n, m, adjacency_matrix, edges); //reading

	if (!check_graph(adjacency_matrix, n))
	{
		fprintf(fout, "no spanning tree");
		free(adjacency_matrix);
		return 0;
	}
	qsort(edges, m, sizeof(TEdge), comparing_func);

	int *flags = (int*)malloc(n * sizeof(int));

	int i, j;
	for (i = 0; i < n; i++) //enumeration
		flags[i] = -1; //not visited

	for (i = 0; i < m; i++)
	{
		int vert1 = edges[i].vertex_1;
		int vert2 = edges[i].vertex_2;
		if ((flags[vert1] == flags[vert2]) && (flags[vert1] != -1)) //same colour and were visited
			continue;
		if ((flags[vert1] != -1) && (flags[vert2] != -1)) //different colours and were visited
		{
			int flags_vert2 = flags[vert2]; //buffer
			for (j = 0; j < n; j++)
			if (flags[j] == flags_vert2)
				flags[j] = flags[vert1]; //changing of the colours for mutural one (which was one of previous)
			print_edge(fout, vert1, vert2);
		}
		else
		if ((flags[vert1] == -1) && (flags[vert2] != -1)) //if one of them wasn't visited
		{
			flags[vert1] = flags[vert2]; //repainting in the second color 
			print_edge(fout, vert1, vert2);
		}
		else
		if ((flags[vert1] != -1) && (flags[vert2] == -1)) //if another wasn't visited
		{
			flags[vert2] = flags[vert1]; //repainting in the first color
			print_edge(fout, vert1, vert2);
		}
		else
		if ((flags[vert1] == -1) && (flags[vert2] == -1)) //if both of them weren't visited
		{
			print_edge(fout, vert1, vert2);
			flags[vert1] = flags[vert2] = vert1; //repainting in the color of first index
		}
	}

	return 0;
}