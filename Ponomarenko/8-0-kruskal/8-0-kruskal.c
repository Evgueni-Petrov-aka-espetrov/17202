#include <stdio.h>
#include <limits.h>

typedef struct TEdge {
	int v1;
	int v2;
	int length;
} Edge;

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}
int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}
Edge* kraskal (Edge* edges, int NumVer,int NumEd) {
	int* color_of_ver = calloc(NumVer + 1, sizeof(int));
	int color_num = 0;
	Edge* result = malloc(sizeof(Edge)*(NumVer-1));
	int res_num = 0;

	for (int i = 0; i < NumEd; i++) {
		int color1 = color_of_ver[edges[i].v1];
		int color2 = color_of_ver[edges[i].v2];
		if (color1 != 0 && color2 != 0 && color1 == color2) {
			continue;
		}

		if (color1 == 0 && color2 == 0) {
			color_num++;
			color_of_ver[edges[i].v1] = color_num;
			color_of_ver[edges[i].v2] = color_num;
		}
		if (color1 != color2) {
			if (color1 == 0) {
				color_of_ver[edges[i].v1] = color2;
			} else if (color2 == 0) {
				color_of_ver[edges[i].v2] = color1;
			}
			else {
				int color_min = min(color1, color2);
				int color_max = max(color1, color2);

				for (int j = 0; j <= NumVer; j++) {
					if (color_of_ver[j] == color_max) {
						color_of_ver[j] = color_min;
					}
				}
			}
		}
		result[res_num] = edges[i];
		res_num++;
	}

	int ch_color = 0;
	for (int i = 0; i < NumVer; ++i) {
		if (color_of_ver[i] == 1) {
			++ch_color;
		}
	
	}
	if (ch_color != NumVer - 1 || (res_num < NumVer - 1) || (NumVer == 0))  {
		printf("no spanning tree");
		exit(0);
	}
	free(color_of_ver);
	return result;
}

void swap(Edge* a,Edge* b) {
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


int main() {
	int NumVer;
	int NumEd;
	scanf("%d ", &NumVer);
	if (NumVer > 5000) {
		printf("bad number of vertices");
		exit(0);
	}
	scanf("%d ", &NumEd);
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
		if ((0 > edges[i].length) || (edges[i].length > INT_MAX))  {
			printf("bad length");
			exit(0);
		
		}
		if (edges[i].v1 == edges[i].v2) {
			--NumEd;
			--i;
		}
	}
	sort(edges, NumEd);
	Edge* result = kraskal(edges, NumVer, NumEd);
	for (int i = 0; i < NumVer - 1; i++) {
		printf("%d %d \n", min(result[i].v1, result[i].v2), max(result[i].v1, result[i].v2) );
	}

	free(result);
	free(edges);
	return 0;
}
