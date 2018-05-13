#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef struct TEdge {
	int v1;
	int v2;
	int length;
} Edge;

typedef struct Node {
	int ver;
	int data;
	struct Node* next;
} Node;

typedef struct Stack {
	Node* first;
} Stack;


typedef struct Ver_Table {
	Stack** ver_in;
} Ver_Table;


void free_node(Node* node) {
	if (node == NULL) {
		return;
	}
	else {
		free_node(node->next);
		free(node);
	}
}

void free_stack(Stack* stack) {
	free_node(stack->first);
	free(stack);
}

void free_table(Ver_Table* table, int NumVer) {
	for (int i = 1; i <= NumVer; ++i) {
		free_stack(table->ver_in[i]);
	}
	free(table);
}

void check_malloc(void* ptr) {
	if (ptr == NULL) {
		printf("not enough memory");
		exit(0);
	}
}

Stack* new_stack() {
	Stack* help = (Stack*)malloc(sizeof(Stack));
	check_malloc(help);
	help->first = NULL;
	return help;
}

Node* push_node(int ver, int value) {
	Node* help = (Node*)malloc(sizeof(Node));
	check_malloc(help);
	help->data = value;
	help->ver = ver;
	help->next = NULL;
	return help;
}


void push_stack(Stack* stack, int ver, int value) {
	Node* help = push_node(ver, value);
	help->next = stack->first;
	stack->first = help;
}

int pop_stack(Stack* stack) {
	Node* help = stack->first;
	int value = help->data;
	stack->first = stack->first->next;
	free(help);
	return value;
}

void push_edge(Ver_Table* table, Edge edge) {
	push_stack(table->ver_in[edge.v1], edge.v2, edge.length);
	push_stack(table->ver_in[edge.v2], edge.v1, edge.length);

};

Ver_Table* new_table(int Num_Ver) {

	Ver_Table* help = (Ver_Table*)malloc(sizeof(Ver_Table));

	check_malloc(help);
	help->ver_in = (Stack**)malloc(sizeof(Stack*)*(Num_Ver + 1));
	check_malloc(help->ver_in);
	for (int i = 1; i <= Num_Ver; ++i) {
		help->ver_in[i] = new_stack();
	}

	return help;
}




int* dijkstra_path(Ver_Table* table, const int NumVer, const int NumEd, long long* short_way, const int PathBeg, const int PathEnd) {
	int* colors = calloc(NumVer + 1, sizeof(int));
	int* previous_for_way = calloc(NumVer + 1, sizeof(int));
	//int NumWay = 0;
	int cur_ver = PathBeg;

	for (int i = 1; i <= NumVer; ++i) {
		Node* tmp = table->ver_in[cur_ver]->first;


		while (tmp != NULL) {

			int edge_to_ver = tmp->ver;
			int edge_length = tmp->data;

			if ((short_way[edge_to_ver] >= (short_way[cur_ver] + edge_length))
				&& (colors[edge_to_ver] != 1)) {

				short_way[edge_to_ver] = short_way[cur_ver] + edge_length;
				previous_for_way[edge_to_ver] = cur_ver;
			}
			tmp = tmp->next;
		}


		colors[cur_ver] = 1;

		long long min_len = LLONG_MAX;
		int min_to = 0;

		for (int j = 1; j <= NumVer; ++j) {
			if ((short_way[j] <= min_len) && (colors[j] != 1)) {
				min_len = short_way[j];
				min_to = j;
			}
		}

		if (min_to != 0) {
			cur_ver = min_to;
		}
		else {
			return previous_for_way;
		}

			}

	}

void is_cyclous (int Tmp_Ver, Ver_Table* table, int* color, int previous) {
	if (color[Tmp_Ver] == 0) {
		color[Tmp_Ver] = 1;

		Node* tmp = table->ver_in[Tmp_Ver]->first;

		while (tmp != NULL) {
			

			if (tmp->ver != previous) {
				is_cyclous(tmp->ver, table, color, Tmp_Ver);
			}
			tmp = tmp->next;
		}
		color[Tmp_Ver] = 2;
		return;
	}
	else if (color[Tmp_Ver] == 1) {
		printf("overflow");
		exit(0);
	}
}


void check_(int NumVer,int PathBeg, Ver_Table* table) {

	int* color_for_check = calloc((NumVer + 1),sizeof(int));

	is_cyclous( PathBeg, table, color_for_check, PathBeg);

	free(color_for_check);
}

int main() {
	int NumVer;
	int NumEd;
	int PathBeg, PathEnd;
	if (scanf("%d%d%d%d", &NumVer, &PathBeg, &PathEnd, &NumEd) != 4) {
		printf("bad number of lines");
		exit(0);
	}


	if (NumVer > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	Ver_Table* table = new_table(NumVer);
	if ((PathBeg > NumVer) || (PathEnd > NumVer) || (PathBeg <= 0) || (PathEnd <= 0)) {
		printf("bad vertex");
		return 0;
	}
	if (NumEd > NumVer*(NumVer + 1) / 2) {
		printf("bad number of edges");
		exit(0);
	}



	Edge* edges = malloc(sizeof(Edge)*NumEd);

	for (int i = 0; i < NumEd; ++i) {
		if (scanf("%d%d%d", &edges[i].v1, &edges[i].v2, &edges[i].length) < 3) {
			printf("bad number of lines");
			exit(0);
		};

		if ((edges[i].v1 > NumVer) || (edges[i].v2 > NumVer) || (edges[i].v1 < 0) || (edges[i].v2 < 0)) {
			printf("bad vertex");
			exit(0);
		}
		if ((0 > edges[i].length) || (edges[i].length > INT_MAX)) {
			printf("bad length");
			exit(0);

		}
		if (edges[i].v1 == edges[i].v2) {
			--NumEd;
			--i;
		}
		else {
			push_edge(table, edges[i]);
		}
	}

	

	long long* short_way = malloc(sizeof(long long)*(NumVer + 1));
	for (int i = 1; i <= NumVer; ++i) {
		if (i != PathBeg) short_way[i] = LLONG_MAX;
		else short_way[i] = 0;
	}


	int* result = dijkstra_path(table, NumVer, NumEd, short_way, PathBeg, PathEnd);


	for (int i = 1; i <= NumVer; ++i) {
		if (short_way[i] == LLONG_MAX) {
			printf("oo ");
		}
		else if (short_way[i] > INT_MAX) {
			printf("INT_MAX+ ");
		}
		else {
			printf("%d ", short_way[i]);
		}
	}

	int cur_way_ver = PathEnd;



	printf("\n");
	if (short_way[PathEnd] == LLONG_MAX) {
		printf("no path");
	} else {
		if (short_way[PathEnd] > INT_MAX) {
			check_(NumVer, PathBeg, table);
		}
		while (cur_way_ver != 0) {
			printf("%d ", cur_way_ver);
			cur_way_ver = result[cur_way_ver];
		}
	}

	free_table(table,NumVer);
	free(result);
	free(short_way);
	return 0;
}
