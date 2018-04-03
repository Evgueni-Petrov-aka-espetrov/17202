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
struct TTreeNode* TTreeNode_Balance(struct TTreeNode* item,int w,int z)
{
	//Обновить данные о высоте узла на основе прямых потомков
	TTreeNode_CalcHeight(item);
	//Если излишний перевес вправо (справа максимальная глубина больше)
	if (TTreeNode_GetBalanceFactor(item) == 2)
	{
		//Если у правого потомка перекос в противоположную сторону, значит надо применить большое левое вращение (которое состоит из последовательного правого и левого вращений)
		if (TTreeNode_GetBalanceFactor(item->right) < 0){
            if(w)
                printf("%iR",z);
			item->right = TTreeNode_RotateRight(item->right);
		}
		//При любом варианте надо сделать левое вращение
		if(w)
            printf("%iL",z);
		return TTreeNode_RotateLeft(item);
	}
	//Если излишний перевес влево (слева максимальная глубина больше)
	if (TTreeNode_GetBalanceFactor(item) == -2)
	{
		//Если у левого потомка перекос в противоположную сторону, значит надо применить большое правое вращение (которое состоит из последовательного левого и правого вращений)
		if (TTreeNode_GetBalanceFactor(item->left) > 0){
            if(w)
                printf("%iL",z);
			item->left = TTreeNode_RotateLeft(item->left);
		}
		//При любом варианте надо сделать правое вращение
		if(w)
		    printf("%iR",z);
		return TTreeNode_RotateRight(item);

	}
	//Дизбаланс меньше 2х не надо выправлять, поэтому просто возвращаем узел без изменений, а больше не может быть из-за устройства дерева
	return item;
}

//Вставка узла в дерево
struct TTreeNode* TTreeNode_Insert(struct TTreeNode* item, int val,int u,int z)
{
	//Если нашли пустое место в дереве для узла, создаем новый узел, обнуляем его данные и возвращаем указатель на него
	if (!item)
	{
	    if(u)
            printf("m");
		item = (struct TTreeNode*) malloc(sizeof(struct TTreeNode));
		item->height = 1;
		item->val = val;
		item->left = item->right = NULL;
		return item;
	}
	//Иначе сравниваем вставляемое значение со значением в узле, в зависимости от чего идём рекурсивно искать место для вставки влево или вправо
	if (val>item->val){
        if(u)
            printf("r");
		item->right = TTreeNode_Insert(item->right, val,u,z + 1);
	}
	else{
        if(u)
            printf("l");
		item->left = TTreeNode_Insert(item->left, val,u,z + 1);
	}
	//После каждой из вставок мы возвращаемся от вставленного узла к корню, каждый узел на пути балансируем, в результате всё дерево останется сбалансированным
	return TTreeNode_Balance(item,u,z);
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
struct tree{
    int data,height;
    struct tree *left;
    struct tree *right;
};
void right_turn(struct tree **h){
    struct tree *g = *h;
    *h = (*h)->left;
    g->left = (*h)->right;
    (*h)->right = g;
    if((*h)->right->left){
        if((*h)->right->right){
            if((*h)->right->left < (*h)->right->right)
                (*h)->right->height = (*h)->right->right->height + 1;
            else
                (*h)->right->height = (*h)->right->left->height + 1;
        }
        else
            (*h)->right->height = (*h)->right->left->height + 1;
    }
    else{
        if((*h)->right->right)
            (*h)->right->height = (*h)->right->right->height + 1;
        else
            (*h)->right->height = 1;
    }
    if((*h)->left){
        if((*h)->right){
            if((*h)->left < (*h)->right)
                (*h)->height = (*h)->right->height + 1;
            else
                (*h)->height = (*h)->left->height + 1;
        }
        else
            (*h)->height = (*h)->left->height + 1;
    }
    else{
        if((*h)->right)
            (*h)->height = (*h)->right->height + 1;
        else
            (*h)->height = 1;
    }
}
void left_turn(struct tree **m){
    struct tree *l = *m;
    *m = (*m)->right;
    l->right = (*m)->left;
    (*m)->left = l;
    if((*m)->left->left){
        if((*m)->left->right){
            if((*m)->left->left < (*m)->left->right)
                (*m)->left->height = (*m)->left->right->height + 1;
            else
                (*m)->left->height = (*m)->left->left->height + 1;
        }
        else
            (*m)->left->height = (*m)->left->left->height + 1;
    }
    else{
        if((*m)->left->right)
            (*m)->left->height = (*m)->left->right->height + 1;
        else
            (*m)->left->height = 1;
    }
    if((*m)->left){
        if((*m)->right){
            if((*m)->left < (*m)->right)
                (*m)->height = (*m)->right->height + 1;
            else
                (*m)->height = (*m)->left->height + 1;
        }
        else
            (*m)->height = (*m)->left->height + 1;
    }
    else{
        if((*m)->right)
            (*m)->height = (*m)->right->height + 1;
        else
            (*m)->height = 1;
    }
}
int inserting(int c,struct tree **p,int t,int y){
    struct tree *d = *p;
    if(d == NULL){
        if(t)
            printf("m");
        *p = malloc(sizeof(struct tree));
        d = *p;
        d->data = c;
        d->height = 1;
        d->left = NULL;
        d->right = NULL;
        return 1;
    }
    int e,f;
    if(c < d->data){
        if(t)
            printf("l");
        e = inserting(c,&(d->left),t,y + 1);
        if(d->right)
            f = d->right->height;
        else
            f = 0;
        if((e == f + 1))
            d->height = e + 1;
        if(e == f + 2){
            if(d->left->left)
                e = d->left->left->height;
            else
                e = 0;
            if(d->left->right)
                f = d->left->right->height;
            else
                f = 0;
            if(e > f){
                if(t)
                    printf("&iR",y);
                right_turn(p);
            }
            else{
                if(t)
                    printf("%iL%iR",y,y);
                left_turn(&(d->left));
                right_turn(p);
            }
        }
    }
    else{
        if(t)
            printf("r");
        e = inserting(c,&(d->right),t,y + 1);
        if(d->left)
            f = d->left->height;
        else
            f = 0;
        //printf("%i %i\n",e,d->left);
        if((e == f + 1))
            d->height = e + 1;
        if(e == f + 2){
            if(d->right->right)
                e = d->right->right->height;
            else
                e = 0;
            if(d->right->left)
                f = d->right->left->height;
            else
                f = 0;
            if(e > f){
                if(t)
                    printf("%iL",y);
                left_turn(p);
            }
            else{
                if(t)
                    printf("%iR%iL",y,y);
                right_turn(&(d->right));
                left_turn(p);
            }
        }
    }
    return d->height;
}
int r(struct TTreeNode *p, struct tree *q,int v){
    if(TTreeNode_GetHeight(p) != q->height){
        if(v)
            printf("1");
        return 1;
    }
    if(p->left){
        if(q->left){
            if(r(p->left,q->left,v)){
                if(v)
                    printf("2");
                return 1;
            }
        }
        else{
            if(v)
                printf("3");
            return 1;
        }
    }
    else
        if(q->left){
            if(v)
                printf("4");
            return 1;
        }
    if(p->right){
        if(q->right){
            if(r(p->right,q->right,v)){
                if(v)
                    printf("5");
                return 1;
            }
        }
        else{
            if(v)
                printf("6");
            return 1;
        }
    }
    else
        if(q->right){
            if(v)
                printf("7");
            return 1;
        }
    return 0;
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
	struct TTreeNode *x = NULL;
	int z,i;
	//Пока файл не закончился, считываем очередное число и вставляем его в дерево
	struct tree *b = NULL;
	int s;
	for (i=0;i<n;i++){
        if(i == 511)
            s = 1;
        else
            s = 0;
		fscanf(in, "%d", &z);
		x = TTreeNode_Insert(x, z,s,1);
		if(s)
            printf("\n");
		inserting(z,&b,s,1);
		if(s)
		    printf("\n");
		//if(i == 1)
            //printf("%i %i %i %i\n",x->height,b->height,x->right->height,b->right->height);
        if(r(x,b,s) && (i < 512))
            printf("\n%i %i %i %i %i\n",i,TTreeNode_GetHeight(x),b->height,TTreeNode_GetHeight(x->right),b->right->height);
	}
	fclose(in);
	FILE *out = fopen("out.txt", "w");
	fprintf(out, "%d", TTreeNode_GetHeight(x));
	fclose(out);
	TTreeNode_Free(x);
	scanf("%d", &z);
	return 0;
}
