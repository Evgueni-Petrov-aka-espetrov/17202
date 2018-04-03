#include <stdio.h>
#include <stdlib.h>
struct tree{
    int data,height;
    struct tree *left;
    struct tree *right;
};
void left_turn(struct tree **h){
    struct tree *g = *h;
    *h = (*h)->left;
    g->left = (*h)->right;
    (*h)->right = g;
    if((*h)->right->left){
        if((*h)->right->right){
            if((*h)->right->left->height < (*h)->right->right->height)
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
            if((*h)->left->height < (*h)->right->height)
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
void right_turn(struct tree **m){
    struct tree *l = *m;
    *m = (*m)->right;
    l->right = (*m)->left;
    (*m)->left = l;
    if((*m)->left->left){
        if((*m)->left->right){
            if((*m)->left->left->height < (*m)->left->right->height)
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
            if((*m)->left->height < (*m)->right->height)
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
