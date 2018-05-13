#include <stdio.h>
#include <stdlib.h>

//��������� ���� ������: �������� ����, ����� � ������ ������� � ���������� �� ������ �������� ������� (�������������)
struct TTreeNode {
	int val;
	unsigned char height;
	struct TTreeNode *left;
	struct TTreeNode *right;
};

//������� ��� ���� item->height, ��������� ���������� ��� ������� ���������
unsigned char TTreeNode_GetHeight(struct TTreeNode *item)
{
	return item ? item->height : 0;
}

//������� ������������������ ���� (������� ����� ������������ �������� ������ � ������� ���������
int TTreeNode_GetBalanceFactor(struct TTreeNode *item)
{
	return TTreeNode_GetHeight(item->right) - TTreeNode_GetHeight(item->left);
}

//������� ������������ ������� �������� ���� �� ������ ������������ ������� ������ � ������� ������� �������
void TTreeNode_CalcHeight(struct TTreeNode* item)
{
	unsigned char hl = TTreeNode_GetHeight(item->left);
	unsigned char hr = TTreeNode_GetHeight(item->right);
	item->height = (hl<hr ? hr : hl) + 1;
}

//�������, ������� �������� � ���������� �������� ������ ��������� ��� �������� � ���������� ����� ������ (������� ����� ����������)
//������ ������� ������������ ���� item
struct TTreeNode* TTreeNode_RotateRight(struct TTreeNode* item) 
{
	struct TTreeNode* other = item->left;
	item->left = other->right;
	other->right = item;
	TTreeNode_CalcHeight(item);
	TTreeNode_CalcHeight(other);
	return other;
}

//����� ������� ������������ ���� item
struct TTreeNode* TTreeNode_RotateLeft(struct TTreeNode* item) 
{
	struct TTreeNode* other = item->right;
	item->right = other->left;
	other->left = item;
	TTreeNode_CalcHeight(item);
	TTreeNode_CalcHeight(other);
	return other;
}

//������������ ���� item
//������ ������������������ �� ����� ���� �� ������ ������ 2, ������ ��� ��� ��������� ���� ��������� ����������������� � ������ ���������� 1
struct TTreeNode* TTreeNode_Balance(struct TTreeNode* item) 
{
	//�������� ������ � ������ ���� �� ������ ������ ��������
	TTreeNode_CalcHeight(item);
	//���� �������� ������� ������ (������ ������������ ������� ������)
	if (TTreeNode_GetBalanceFactor(item) == 2)
	{
		//���� � ������� ������� ������� � ��������������� �������, ������ ���� ��������� ������� ����� �������� (������� ������� �� ����������������� ������� � ������ ��������)
		if (TTreeNode_GetBalanceFactor(item->right) < 0)
			item->right = TTreeNode_RotateRight(item->right);
		//��� ����� �������� ���� ������� ����� ��������
		return TTreeNode_RotateLeft(item);
	}
	//���� �������� ������� ����� (����� ������������ ������� ������)
	if (TTreeNode_GetBalanceFactor(item) == -2)
	{
		//���� � ������ ������� ������� � ��������������� �������, ������ ���� ��������� ������� ������ �������� (������� ������� �� ����������������� ������ � ������� ��������)
		if (TTreeNode_GetBalanceFactor(item->left) > 0)
			item->left = TTreeNode_RotateLeft(item->left);
		//��� ����� �������� ���� ������� ������ ��������
		return TTreeNode_RotateRight(item);

	}
	//��������� ������ 2� �� ���� ����������, ������� ������ ���������� ���� ��� ���������, � ������ �� ����� ���� ��-�� ���������� ������
	return item; 
}

//������� ���� � ������
struct TTreeNode* TTreeNode_Insert(struct TTreeNode* item, int val)
{
	//���� ����� ������ ����� � ������ ��� ����, ������� ����� ����, �������� ��� ������ � ���������� ��������� �� ����
	if (!item)
	{
		item = (struct TTreeNode*) malloc(sizeof(struct TTreeNode));
		item->height = 1;
		item->val = val;
		item->left = item->right = NULL;
		return item;
	}
	//����� ���������� ����������� �������� �� ��������� � ����, � ����������� �� ���� ��� ���������� ������ ����� ��� ������� ����� ��� ������
	if (val>item->val)
		item->right = TTreeNode_Insert(item->right, val);
	else
		item->left = TTreeNode_Insert(item->left, val);
	//����� ������ �� ������� �� ������������ �� ������������ ���� � �����, ������ ���� �� ���� �����������, � ���������� �� ������ ��������� ����������������
	return TTreeNode_Balance(item);
}

//������ ������
void TTreeNode_Print(struct TTreeNode *item) {
	printf("(");
	//���� ���� ����� ��������� ������� ���
	if (item->left) {
		TTreeNode_Print(item->left);
		printf("<=");
	}
	//������� �������� � ����
	printf("%d", item->val);
	//���� ���� ������ ��������� ������� ���
	if (item->right) {
		printf("=>");
		TTreeNode_Print(item->right);
	}
	printf(")");
}

//����������� ������������ ������, ���������� ��� ������
void TTreeNode_Free(struct TTreeNode *item)
{
	if (!item) return;
	//����������� ����� � ������ ����������
	TTreeNode_Free(item->left);
	TTreeNode_Free(item->right);
	//����� ����� ����� ���������� ������, ���������� ��� ���� ������� 
	free(item);
}
int main()
{
	//��������� ������� ����, ��� ������ ��������� ����������
	FILE *in = fopen("in.txt", "r");
	if (!in)
	{
		printf("Can't open input file!\n");
		return 0;
	}
	int n;
	fscanf(in, "%d", &n);
	//���������� ������ ������
	TTreeNode *x = NULL;
	int z;
	//���� ���� �� ����������, ��������� ��������� ����� � ��������� ��� � ������
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