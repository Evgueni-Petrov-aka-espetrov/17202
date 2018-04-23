#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

void swap(int *a, int *b) {
	int buffer = *a;
	*a = *b;
	*b = buffer;
	return;
}

void makeHeap(int* arr, const int n, const int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest])
		largest = left;

	if (right < n && arr[right] > arr[largest])
		largest = right;

	if (largest != i)
	{
		swap(&arr[i], &arr[largest]);
		makeHeap(arr, n, largest);
	}
}

void heapSort(int *arr, const int n)
{
	for (int i = (n / 2) - 1; i > -1; --i) {
		makeHeap(arr, n, i);
	}

	for (int i = n - 1; i > -1; i--)
	{
		swap(&arr[0], &arr[i]);
		makeHeap(arr, i, 0);
	}
}

void readArray(ifstream *fin, int *massToRead, const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		scanf("%d", &massToRead[0] + i);
	}
}
void writeResult(const int *massToWrite, const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		printf("%d", massToWrite[i]);
		if (i == (massSize - 1)) {
			break;
		}
		printf(" ");
	}
}

int main()
{
	ifstream fin("in.txt");
	int massSize;
	scanf("%d", &massSize);
	int *arr = (int*)malloc(sizeof(int)*(massSize));
	readArray(&fin, &arr[0], massSize);
	heapSort(arr, massSize);
	writeResult(arr, massSize);
	//system("pause");
	free(arr);
}
