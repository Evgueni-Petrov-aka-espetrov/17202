#include <stdio.h>
#include <stdlib.h>

#define BLACK 2
#define GREY 1
#define WHITE 0

typedef struct Node {
	int data;
	Node* next;
} Node;

typedef struct Stack {
	Node* first;
} Stack ;


typedef struct Ver_Table{
	Stack** ver_in;
	int* color;
} Ver_Table;

typedef struct TEdge {
	int v1;
	int v2;
} Edge;

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

void free_table(Ver_Table* table,int NumVer) {
	for (int i = 1; i <= NumVer; ++i) {
		free_stack(table->ver_in[i]);
	}
	free(table->color);
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

Node* push_node(int value) {
	Node* help = (Node*)malloc(sizeof(Node));
	check_malloc(help);
	help->data = value;
	help->next = NULL;
	return help;
}


void push_stack(Stack* stack, int value) {
	Node* help = push_node(value);
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
	push_stack(table->ver_in[edge.v1], edge.v2);
};

Ver_Table* new_table(int Num_Ver) {

	Ver_Table* help =(Ver_Table*) malloc(sizeof(Ver_Table));
	
	check_malloc(help);
	help->ver_in = (Stack**) malloc(sizeof(Stack*)*(Num_Ver + 1));
	check_malloc(help->ver_in);
	for (int i = 1; i <= Num_Ver; ++i) {
		help->ver_in[i] = new_stack();
	}
	
	help->color = (int*) calloc(Num_Ver + 1, sizeof(int));
	check_malloc(help->color);

	return help;
}


void top_sort(int Tmp_Ver, Ver_Table* table, Stack* stack_res) {
	if (table->color[Tmp_Ver] == WHITE) {
		table->color[Tmp_Ver] = GREY;
		
		while (table->ver_in[Tmp_Ver]->first != NULL) {
			top_sort(pop_stack(table->ver_in[Tmp_Ver]), table, stack_res);
		}
		table->color[Tmp_Ver] = BLACK;
		
		push_stack( stack_res, Tmp_Ver);
		return;
	}
	else if (table->color[Tmp_Ver] == GREY) {
		printf("impossible to sort");
		exit(0);
	}
}

int main() {
	Stack* stack = new_stack();
	int NumVer;
	int NumEd;

	if (scanf("%d ", &NumVer) == 1) {
		if (NumVer > 1000) {
			printf("bad number of vertices");
			exit(0);
		}
	}
	else {
		printf("bad number of lines");
		exit(0);
	}

	if (scanf("%d ", &NumEd) == 1) {
		if (NumEd > NumVer*(NumVer + 1) / 2) {
			printf("bad number of edges");
			exit(0);
		}
	} else {
		printf("bad number of lines");
		exit(0);
	}

	Ver_Table* table = new_table(NumVer);

	for (int i = 0; i < NumEd; ++i) {
		Edge tmp;
		if (scanf("%d%d", &tmp.v1, &tmp.v2) < 2) {
			printf("bad number of lines");
			exit(0);
		};

		if ((tmp.v1 > NumVer) || (tmp.v2 > NumVer) || (tmp.v1 < 0) || (tmp.v2 < 0)) {
			printf("bad vertex");
			exit(0);
		}
		if (tmp.v1 == tmp.v2) {
			printf("impossible to sort");
			exit(0);
		}
		push_edge(table,tmp);
	}

	Stack* res_stack = new_stack();
	for (int i = 1; i <= NumVer; ++i) {
		top_sort(i , table , res_stack);
	}


	while (res_stack->first != NULL) {
		printf("%d ", pop_stack(res_stack));
	}
	free_table(table,NumVer);
	free_stack(res_stack);

	return 0;
}
