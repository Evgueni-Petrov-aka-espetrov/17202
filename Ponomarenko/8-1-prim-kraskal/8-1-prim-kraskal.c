#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef struct TEdge {
	int v1;
	int v2;
	int length;
} Edge;


void swap(Edge* a, Edge* b) {
	Edge help;
	help = *a;
	*a = *b;
	*b = help;
	return;
}

void sort(Edge* array, int num) {
	if ((num == 0) || (num == 1)) { return; };

	int i = 0, j = num - 1;

	int point = array[rand() % num].length;
	do {
		while (point > array[i].length) { i++; }
		while (point < array[j].length) { j--; }
		if (i <= j) {
			swap(&array[i], &array[j]);
			i++;
			j--;
		}
	} while (i <= j);
	sort(array, j + 1);
	sort(array + i, num - i);
	return;
}

Edge* prim_kraskal(Edge* edges, int NumVer, int NumEd) {
	Edge* result = malloc(sizeof(Edge)*(NumVer + 1));


	int* color_ed = (int*)calloc(NumVer + 1, sizeof(int));
	
	result[0] = edges[0];
	color_ed[edges[0].v1] = 1;
	color_ed[edges[0].v2] = 1;
	int i = 1;

	for (i = 1; i < NumVer - 1; ++i) {
		for (int j = 1; j < NumEd; ++j) {
			if (color_ed[edges[j].v1] != color_ed[edges[j].v2]) {
				result[i] = edges[j];
				color_ed[edges[j].v1] = 1;
				color_ed[edges[j].v2] = 1;
				break;
			}
			else if (j == NumEd - 1) {
				printf("no spanning tree");
				exit(0);
			}
		}
		
	}
	return result;
}


int main() {
	int NumVer;
	int NumEd;
	scanf("%d", &NumVer);

	if ((NumVer > 5000) || (NumVer < 0)) {
		printf("bad number of vertices");
		exit(0);
	}

	scanf("%d", &NumEd);
	if ((NumEd > NumVer*(NumVer + 1) / 2) || (NumEd < 0)) {
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
	}

	if (NumEd == 0 && NumVer == 1) {
		return 0;
	}

	if (NumVer == 0 || NumEd == 0 || NumEd < NumVer - 1) {
		printf("no spanning tree");
		return 0;
	}

	sort(edges, NumEd);
	Edge* result = prim_kraskal(edges, NumVer, NumEd);

	for (int i = 0; i < NumVer - 1; i++) {
		printf("%d %d \n", min(result[i].v1, result[i].v2), max(result[i].v1, result[i].v2));
	}

	free(result);
	free(edges);
	return 0;
}
