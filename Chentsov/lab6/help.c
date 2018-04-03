#include <stdio.h>
#include <stdlib.h>
struct tree{
    int data,height;
    struct tree *left;
    struct tree *right;
};
void height_update(struct tree* q){
    if(q->left){
        if(q->right){
            if(q->left->height < q->right->height)
                q->height = q->right->height + 1;
            else
                q->height = q->left->height + 1;
        }
        else
            q->height = q->left->height + 1;
    }
    else{
        if(q->right)
            q->height = q->right->height + 1;
        else
            q->height = 1;
    }
}
void left_turn(struct tree **h){
    struct tree *g = *h;
    *h = (*h)->left;
    g->left = (*h)->right;
    (*h)->right = g;
    height_update((*h)->right);
    height_update(*h);
}
void right_turn(struct tree **m){
    struct tree *l = *m;
    *m = (*m)->right;
    l->right = (*m)->left;
    (*m)->left = l;
    height_update((*m)->left);
    height_update(*m);
}
int inserting(int c,struct tree **p){
    struct tree *d = *p;
    if(d == NULL){
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
        e = inserting(c,&(d->left));
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
            if(e <= f)
                right_turn(&(d->left));
            left_turn(p);
        }
    }
    else{
        e = inserting(c,&(d->right));
        if(d->left)
            f = d->left->height;
        else
            f = 0;
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
            if(e <= f)
                left_turn(&(d->right));
            right_turn(p);
        }
    }
    return d->height;
}
int main(){
    int N;
    scanf("%i",&N);
    int i,a;
    struct tree *b = NULL;
    for(i = 0;i < N;++ i){
        scanf("%i",&a);
        inserting(a,&b);
    }
    if(b){
        printf("%i",b->height);
    }
    else
        printf("0");
    return 0;
}
