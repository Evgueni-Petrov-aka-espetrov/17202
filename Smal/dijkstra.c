#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scan(int* numV, int* numE, int* Start, int* Finish, FILE* oFile, FILE* iFile)
{
	fscanf(iFile, "%d", numV);
	if (*numV < 0 || *numV > 5000)
	{
		fprintf(oFile, "bad number of vertices");
		return 0;
	}

	fscanf(iFile, "%d %d", Start, Finish);
	if (*Start <= 0 || *Start > *numV || *Finish <= 0 || *Finish > *numV)
	{
		fprintf(oFile, "bad vertex");
		return 0;
	}

	fscanf(iFile, "%d", numE);
	if (*numE < 0 || *numE > *numV*(*numV) / 2)
	{
		fprintf(oFile, "bad number of edges");
		return 0;
	}
	(*Start)--;
	(*Finish)--;
	return 1;
}

unsigned int scanLength(FILE* iFile, FILE* oFile)
{
	unsigned int l = 0;
	char c = fgetc(iFile);

	while ((c != '-') && (c != '\n') && (c != EOF))
	{
		l = l * 10 + c - 48;
		c = fgetc(iFile);
	}
	return l;
}

void clearEdges(unsigned int** edges, int numV, FILE* oFile)
{
	for (int i = 0; i < numV; i++)
	{
		free(edges[i]);
	}
	free(edges);
}

unsigned int** fillEdges(int numV, int numE, FILE* iFile, FILE* oFile, int* error)
{
	unsigned int** edges = (unsigned int**)malloc(sizeof(unsigned int*)*(numV));

	for (int i = 0; i < numV; i++)
	{
		edges[i] = (unsigned int*)malloc(sizeof(unsigned int)*(numV));
		memset(edges[i], 0, sizeof(unsigned int)*(numV));
	}

	for (int i = 0; i < numE; i++)
	{
		int V1, V2;
		unsigned int length = 0;


		if (feof(iFile))
		{
			*error = 1;
			return NULL;
		}
		if (fscanf(iFile, "%d %d ", &V1, &V2) < 1)
		{
			*error = 1;
			return NULL;
		}


		if (numV < V1 || V1 <= 0 || numV < V2 || V2 <= 0)
		{
			*error = 2;
			return NULL;
		}

		length = scanLength(iFile, oFile);



		if (length <= 0 || length > INT_MAX)
		{
			*error = 3;
			return NULL;
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

void ClearAll(unsigned int ** edges, long int* MinDistance,
	int* Path, int* AlreadyIn, int* DisOverflow, int numV, FILE* oFile)
{
	clearEdges(edges, numV, oFile);
	free(Path);
	free(MinDistance);
	free(AlreadyIn);
	free(DisOverflow);
}

void MinDistanceUpdate(unsigned int ** edges, long int* MinDistance,
	int newV, int* Path, int* AlreadyIn, int numV, FILE* oFile, int* DisOverflow)
{

	if (MinDistance[Path[newV]] + edges[Path[newV]][newV] > LONG_MAX)
	{
		MinDistance[newV] = LONG_MAX;
		DisOverflow[newV] = 1;
	}
	else
		MinDistance[newV] = MinDistance[Path[newV]] + edges[Path[newV]][newV];

	for (int i = 0; i < numV; i++)
	{

		if (!AlreadyIn[i])
		{
			if (edges[newV][i])
			{
				if (MinDistance[i] < 0 || MinDistance[i] > MinDistance[newV] + edges[newV][i])
				{
					if (MinDistance[newV] + edges[newV][i] > INT_MAX || DisOverflow[newV] == 1) {
						MinDistance[i] = LONG_MAX;
						DisOverflow[i] = 1;
					}

					else
						MinDistance[i] = MinDistance[newV] + edges[newV][i];
					Path[i] = newV;
				}
			}
		}

		if ((edges[newV][i]) && (AlreadyIn[i]))
		{
			AlreadyIn[newV]++;
		}
	}
}

int overflowCheck(unsigned int ** edges, int* Path, int Finish, int* AlreadyIn, int numV)
{
	int tmp = Finish;
	if (AlreadyIn[Finish] > 1)
		return 1;

	for (int i = 0; i < numV; i++)
	{
		if ((AlreadyIn[i] > 1) && (edges[i][Finish]))
			return 1;
	}
	return 0;
}

int AddVertex(unsigned int ** edges, long int* MinDistance, int * AlreadyIn, int* Path,
	int* DisOverflow, int numV, FILE* oFile)
{
	long long tmp = LONG_MAX;
	int NewVer = 0;
	int t = 0;
	for (int i = 0; i < numV; i++)
	{
		if (!AlreadyIn[i])
			if ((MinDistance[i] <= tmp) && (MinDistance[i] != -1) && DisOverflow[i] == 0)
			{
				tmp = MinDistance[i];
				NewVer = i;
				t = 1;
			}
	}

	if (!t)
		for (int i = 0; i < numV; i++)
		{
			if (!AlreadyIn[i])
				if ((MinDistance[i] <= tmp) && (MinDistance[i] != -1))
				{
					tmp = MinDistance[i];
					NewVer = i;
					t = 1;
				}
		}

	if (!t)
		return 0;

	MinDistanceUpdate(edges, MinDistance, NewVer, Path, AlreadyIn, numV, oFile, DisOverflow);
	return 1;
}

void printRes(unsigned int ** edges, int* AlreadyIn, int* MinDistance, int* Path,
	int* DisOverflow, int numV, int Start, int Finish, FILE* oFile)
{
	for (int i = 0; i < numV; i++)
	{
		if (DisOverflow[i] == 1)
			fprintf(oFile, "INT_MAX+ ");
		else
		{
			if (MinDistance[i] == -1)
				fprintf(oFile, "oo ");
			else
				fprintf(oFile, "%d ", MinDistance[i]);
		}

	}

	fprintf(oFile, "\n");

	if (DisOverflow[Finish] == 1)
	{
		if (overflowCheck(edges, Path, Finish, AlreadyIn, numV))
			fprintf(oFile, "overflow");
		else
		{
			while (Path[Finish] != Start)
			{
				fprintf(oFile, "%d ", Finish + 1);
				Finish = Path[Finish];
			}
			fprintf(oFile, "%d ", Finish + 1);
			fprintf(oFile, "%d ", Start + 1);
		}

	}
	else
		if (MinDistance[Finish] != -1)
		{
			while (Path[Finish] != Start)
			{
				fprintf(oFile, "%d ", Finish + 1);
				Finish = Path[Finish];
			}
			fprintf(oFile, "%d ", Finish + 1);
			fprintf(oFile, "%d ", Start + 1);

		}
		else
			fprintf(oFile, "no path");

}

int dijkstra(int numV, int numE, int Start, int Finish, FILE* oFile, FILE* iFile)
{
	int error = 0;
	unsigned int** edges = fillEdges(numV, numE, iFile, oFile, &error);

	if (error)
	{
		printErr(error, oFile);
		return 0;
	}

	int * Path = (int*)malloc(sizeof(int) * (numV));
	memset(Path, 0, sizeof(int) * (numV));

	int * AlreadyIn = (int*)malloc(sizeof(int) * (numV));
	memset(AlreadyIn, 0, sizeof(int) * (numV));

	long int * MinDistance = (long int*)malloc(sizeof(long int) * (numV));
	memset(MinDistance, -1, sizeof(long int) * (numV));

	int* DisOverflow = (int*)malloc(sizeof(int) * (numV));
	memset(DisOverflow, 0, sizeof(int) * (numV));

	AlreadyIn[Start] = 1;
	MinDistance[Start] = 0;

	Path[Start] = Start;
	MinDistanceUpdate(edges, MinDistance, Start, Path, AlreadyIn, numV, oFile, DisOverflow);

	while (1)
	{
		if (!AddVertex(edges, MinDistance, AlreadyIn, Path, DisOverflow, numV, oFile))
			break;
	}

	MinDistance[Start] = 0;
	DisOverflow[Start] = 0;

	printRes(edges, AlreadyIn, MinDistance, Path, DisOverflow, numV, Start, Finish, oFile);

	ClearAll(edges, MinDistance, Path, AlreadyIn, DisOverflow, numV, oFile);
}

int main()
{
	FILE* iFile;
	iFile = fopen("in.txt", "r");
	FILE* oFile;
	oFile = fopen("out.txt", "w");

	int numV, numE, Start, Finish;

	if (!scan(&numV, &numE, &Start, &Finish, oFile, iFile))
		return 0;

	dijkstra(numV, numE, Start, Finish, oFile, iFile);
	return 0;
}
