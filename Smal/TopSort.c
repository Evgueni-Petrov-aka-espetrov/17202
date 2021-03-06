﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int test_nums(int numV, int numE, FILE* oFile)
{	
	if (numV < 0 || numV > 1000) 
	{
		fprintf(oFile, "bad number of vertices");
		return 1;
	}

	if (numE < 0 || numE > (numV*(numV + 1) / 2))
	{
		fprintf(oFile, "bad number of edges");
		return 1;
	}

	return 0;
}

int** fillEdges(int numV, int numE, FILE* iFile, FILE* oFile, int* error)
{
	int** edges = (int**)malloc(sizeof(int*)*(numV+1));

	for (int i = 0; i < numV + 1; i++) 
	{
		edges[i] = (int*)malloc(sizeof(int)*(numV + 1));
		memset(edges[i], 0, sizeof(int)*(numV+1));
	}
		
	for (int i = 0; i < numE; i++)
	{
		int V1, V2;
		if (feof(iFile))
		{
			*error = 1;
			return 0;
		}
		if (fscanf(iFile, "%d %d", &V1, &V2) < 1) 
		{
			*error = 1;
			return 0;
		}

		if (numV < V1 || V1 <= 0 || numV < V2 || V2 <= 0)
		{
			*error = 2;
			return 0;
		}
		edges[V1][V2] = 1;
	}

	return edges;
}

void swap(int* t1, int* t2)
{
	int tmp = *t2;
	*t2 = *t1;
	*t1 = tmp;
}

void clear_edges(int** edges, int numV)
{
	for (int i = 0; i < numV + 1; i++)
		free(edges[i]);
}

int* topSort(int numV, int numE, FILE* oFile, FILE* iFile)
{
	int error = 0;
	int** edges = fillEdges(numV, numE, iFile, oFile, &error);
	int* level = (int*)malloc(sizeof(int)*(numV+1));
	for (int i = 0; i < numV + 1; i++)
		level[i] = i;

	if (error == 1) 
	{
		fprintf(oFile, "bad number of lines");
		return 0;
	}
	if (error == 2) {
		fprintf(oFile, "bad vertex");
		return 0;
	}


	for (int i = 1; i < numV + 1; i++)
		for (int j = i + 1; j < numV + 1; j++)
			if (edges[level[j]][level[i]]) {
				swap(&level[j], &level[i]);
			}
	
	for (int i = 1; i < numV + 1; i++)
		for (int j = i; j < numV + 1; j++)
			if (edges[level[j]][level[i]]) 
			{
				clear_edges(edges, numV);
				fprintf(oFile, "impossible to sort");
				return 0;
			}
				
			
	clear_edges(edges, numV);
	
	return level;
}

int prRes(FILE* oFile, int* res, int numV) 
{

	if (!res) 
	{
		free(res);
		return 0;
	}

	for (int i = 1; i < numV + 1; i++)
		fprintf(oFile, "%d ", res[i]);

	free(res);

	return 0;
}

int main()
{
	int numV, numE;
	FILE* iFile;
	FILE* oFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");

	fscanf(iFile, "%d ", &numV);
	if (fscanf(iFile, "%d", &numE) < 1)
	{
		fprintf(oFile, "bad number of lines");
		return 0;
	}
	if (test_nums(numV, numE, oFile))
		return 0;
	
	prRes(oFile, topSort(numV, numE, oFile, iFile), numV);

    return 0;
}

