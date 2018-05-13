#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scan(int* numV, int* numE, FILE* oFile, FILE* iFile)
{
	fscanf(iFile, "%d", numV);
	if (*numV < 0 || *numV > 5000)
	{
		fprintf(oFile, "bad number of vertices");
		return 0;
	}

	fscanf(iFile, "%d", numE);
	if (*numE < 0 || *numE > *numV*(*numV) / 2)
	{
		fprintf(oFile, "bad number of edges");
		return 0;
	}

	if (*numV == 0 && *numE == 0)
	{
		fprintf(oFile, "no spanning tree");
		return 0;
	}
	return 1;
}

void clearEdges(int** edges, int numV)
{
	for (int i = 0; i < numV; i++)
	{
		free(edges[i]);
	}
	free(edges);
}

int** fillEdges(int numV, int numE, FILE* iFile, int* error)
{
	int** edges = (int**)malloc(sizeof(int*)*(numV));

	for (int i = 0; i < numV; i++)
	{
		edges[i] = (int*)malloc(sizeof(int)*(numV));
		memset(edges[i], -1, sizeof(int)*(numV));
	}

	for (int i = 0; i < numE; i++)
	{
		int V1, V2;
		int length = 0;


		if (feof(iFile))
		{
			*error = 1;
			break;
		}

		if (fscanf(iFile, "%d %d ", &V1, &V2) < 1)
		{
			*error = 1;
			break;
		}


		if (numV < V1 || V1 <= 0 || numV < V2 || V2 <= 0)
		{
			*error = 2;
			break;
		}

		fscanf(iFile, "%d", &length);

		if (length <= 0 || length > INT_MAX)
		{
			*error = 3;
			break;
		}

		edges[V1 - 1][V2 - 1] = length;
		edges[V2 - 1][V1 - 1] = length;
	}

	return edges;
}

void printErr(int error, FILE* oFile)
{
	if (error == 1)
		fprintf(oFile, "bad number of lines");

	if (error == 2)
		fprintf(oFile, "bad vertex");

	if (error == 3)
		fprintf(oFile, "bad length");
}

void ClearAll(int ** edges, int* MinDistance,
	int* Path, int* AlreadyIn, int numV)
{
	clearEdges(edges, numV);
	free(Path);
	free(MinDistance);
	free(AlreadyIn);
}

void MinDistanceUpdate(int numV, int newV, int** edges, int* AlreadyIn, int* MinDistance, int* Path)
{
	for (int j = 0; j < numV; ++j)
	{
		if (!AlreadyIn[j] && edges[newV][j] != -1 && (MinDistance[j] > edges[newV][j] || MinDistance[j] == -1))
		{
			Path[j] = newV;
			MinDistance[j] = edges[newV][j];
		}
	}
}

int AddVertex(int numV, int* AlreadyIn, int* MinDistance)
{
	int newV = -1;
	int minD = -1;
	for (int j = 0; j < numV; ++j)
		if (!AlreadyIn[j] && (MinDistance[j] < minD || minD == -1))
		{
			minD = MinDistance[j];
			newV = j;
		}
	AlreadyIn[newV] = 1;
	return newV;
}

int checkEdges(int** edges, int numV) 
{
	int* color = (int*)malloc(sizeof(int)*numV);
	memset(color, 0, sizeof(int) * (numV));
	color[0] = 1;
	for (int i = 0; i < numV; ++i)
		for (int j = 0; j < numV; ++j)
			if (edges[i][j] > 0 && color[i])
				color[j] = 1;
				
			

	for (int i = 0; i < numV; ++i)
		if (!color[i])
		{
			free(color);
			return 0;
		}
	free(color);
	return 1;
}

int Prim_Kruskal(int numV, int numE, FILE* oFile, FILE* iFile)
{
	int error = 0;
	int** edges = fillEdges(numV, numE, iFile, &error);
	
	if (error)
	{
		printErr(error, oFile);
		clearEdges(edges, numV);
		return 0;
	}
	
	if (!checkEdges(edges, numV))
	{
		fprintf(oFile, "no spanning tree");
		clearEdges(edges, numV);
		return 0;
	}

	int * Path = (int*)malloc(sizeof(int) * (numV));
	memset(Path, -1, sizeof(int) * (numV));

	int * AlreadyIn = (int*)malloc(sizeof(int) * (numV));
	memset(AlreadyIn, 0, sizeof(int) * (numV));

	int * MinDistance = (int*)malloc(sizeof(int) * (numV));
	memset(MinDistance, -1, sizeof(int) * (numV));
	
	for (int i = 0; i < numV; ++i)
	{
		int newV = -1;
		newV = AddVertex(numV, AlreadyIn, MinDistance);
		MinDistanceUpdate(numV, newV, edges, AlreadyIn, MinDistance, Path);
		if (MinDistance[newV] != -1)
			fprintf(oFile, "%d %d\n", Path[newV] + 1, newV + 1);
	}
	ClearAll(edges, MinDistance, Path, AlreadyIn, numV);
	return 1;
}

int main()
{
	FILE* iFile;
	iFile = fopen("in.txt", "r");
	FILE* oFile;
	oFile = fopen("out.txt", "w");

	int numV, numE;

	if (!scan(&numV, &numE, oFile, iFile))
		return 0;

	Prim_Kruskal(numV, numE, oFile, iFile);
	return 0;
}
