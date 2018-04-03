#include <stdio.h>
#include <stdlib.h>

//Структура узел дерева: значение узла, левый и правый потомок и расстояние до самого дальнего потомка (высчитывается)
struct TTreeNode {
	int val;
	unsigned char height;
	struct TTreeNode *left;
	struct TTreeNode *right;
};

//Обертка для поля item->height, корректно работающая для пустого поддерева
unsigned char TTreeNode_GetHeight(struct TTreeNode *item)
{
	return item ? item->height : 0;
}

//Рассчет сбалансированности узла (разницы между максимальной глубиной левого и правого поддерева
int TTreeNode_GetBalanceFactor(struct TTreeNode *item)
{
	return TTreeNode_GetHeight(item->right) - TTreeNode_GetHeight(item->left);
}

//Рассчет максимальной глубины потомков узла на основе максимальной глубины левого и правого прямого потомка
void TTreeNode_CalcHeight(struct TTreeNode* item)
{
	unsigned char hl = TTreeNode_GetHeight(item->left);
	unsigned char hr = TTreeNode_GetHeight(item->right);
	item->height = (hl<hr ? hr : hl) + 1;
}

//Функции, которые работают с поворотами получают корень поддерева для поворота и возвращают новый корень (который может измениться)
//Правый поворот относительно узла item
struct TTreeNode* TTreeNode_RotateRight(struct TTreeNode* item) 
{
	struct TTreeNode* other = item->left;
	item->left = other->right;
	other->right = item;
	TTreeNode_CalcHeight(item);
	TTreeNode_CalcHeight(other);
	return other;
}

//Левый поворот относительно узла item
struct TTreeNode* TTreeNode_RotateLeft(struct TTreeNode* item) 
{
	struct TTreeNode* other = item->right;
	item->right = other->left;
	other->left = item;
	TTreeNode_CalcHeight(item);
	TTreeNode_CalcHeight(other);
	return other;
}

//Балансировка узла item
//Фактор сбалансированности не может быть по модулю больше 2, потому что при равенстве двум поддерево перебалансируется и фактор становится 1
struct TTreeNode* TTreeNode_Balance(struct TTreeNode* item) 
{
	//Обновить данные о высоте узла на основе прямых потомков
	TTreeNode_CalcHeight(item);
	//Если излишний перевес вправо (справа максимальная глубина больше)
	if (TTreeNode_GetBalanceFactor(item) == 2)
	{
		//Если у правого потомка перекос в противоположную сторону, значит надо применить большое левое вращение (которое состоит из последовательного правого и левого вращений)
		if (TTreeNode_GetBalanceFactor(item->right) < 0)
			item->right = TTreeNode_RotateRight(item->right);
		//При любом варианте надо сделать левое вращение
		return TTreeNode_RotateLeft(item);
	}
	//Если излишний перевес влево (слева максимальная глубина больше)
	if (TTreeNode_GetBalanceFactor(item) == -2)
	{
		//Если у левого потомка перекос в противоположную сторону, значит надо применить большое правое вращение (которое состоит из последовательного левого и правого вращений)
		if (TTreeNode_GetBalanceFactor(item->left) > 0)
			item->left = TTreeNode_RotateLeft(item->left);
		//При любом варианте надо сделать правое вращение
		return TTreeNode_RotateRight(item);

	}
	//Дизбаланс меньше 2х не надо выправлять, поэтому просто возвращаем узел без изменений, а больше не может быть из-за устройства дерева
	return item; 
}

//Вставка узла в дерево
struct TTreeNode* TTreeNode_Insert(struct TTreeNode* item, int val)
{
	//Если нашли пустое место в дереве для узла, создаем новый узел, обнуляем его данные и возвращаем указатель на него
	if (!item)
	{
		item = (struct TTreeNode*) malloc(sizeof(struct TTreeNode));
		item->height = 1;
		item->val = val;
		item->left = item->right = NULL;
		return item;
	}
	//Иначе сравниваем вставляемое значение со значением в узле, в зависимости от чего идём рекурсивно искать место для вставки влево или вправо
	if (val>item->val)
		item->right = TTreeNode_Insert(item->right, val);
	else
		item->left = TTreeNode_Insert(item->left, val);
	//После каждой из вставок мы возвращаемся от вставленного узла к корню, каждый узел на пути балансируем, в результате всё дерево останется сбалансированным
	return TTreeNode_Balance(item);
}

//Печать дерева
void TTreeNode_Print(struct TTreeNode *item) {
	printf("(");
	//Если есть левое поддерево выводим его
	if (item->left) {
		TTreeNode_Print(item->left);
		printf("<=");
	}
	//Выводим значение в узле
	printf("%d", item->val);
	//Если есть правое поддерево выводим его
	if (item->right) {
		printf("=>");
		TTreeNode_Print(item->right);
	}
	printf(")");
}

//Рекурсивное освобождение памяти, выделенной под дерево
void TTreeNode_Free(struct TTreeNode *item)
{
	if (!item) return;
	//Освобождаем левое и правое поддеревья
	TTreeNode_Free(item->left);
	TTreeNode_Free(item->right);
	//После этого можно освободить память, выделенную под саму вершину 
	free(item);
}
int main()
{
	//Открываем входной файл, при ошибке завершаем выполнение
	FILE *in = fopen("in.txt", "r");
	if (!in)
	{
		printf("Can't open input file!\n");
		return 0;
	}
	int n;
	fscanf(in, "%d", &n);
	//Изначально дерево пустое
	TTreeNode *x = NULL;
	int z;
	//Пока файл не закончился, считываем очередное число и вставляем его в дерево
	for (int i=0;i<n;i++){
		fscanf(in, "%d", &z);
		x = TTreeNode_Insert(x, z);
	}
	fclose(in);
	FILE *out = fopen("out.txt", "w");
	fprintf(out, "%d", TTreeNode_GetHeight(x));
	fclose(out);
	TTreeNode_Free(x);
	scanf("%d", &z);
	return 0;
}