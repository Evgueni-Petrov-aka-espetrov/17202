#include <stdio.h>
#include <stdlib.h>
struct tree{
    int data,height;
    struct tree *left;
    struct tree *right;
};
void height_update(struct tree* tree){
    if(tree->left){
        if(tree->right){
            if(tree->left->height < tree->right->height)
                tree->height = tree->right->height + 1;
            else
                tree->height = tree->left->height + 1;
        }
        else
            tree->height = tree->left->height + 1;
    }
    else{
        if(tree->right)
            tree->height = tree->right->height + 1;
        else
            tree->height = 1;
    }
}
void left_turn(struct tree **tree){
    struct tree *old_base = *tree;
    *tree = (*tree)->left;
    old_base->left = (*tree)->right;
    (*tree)->right = old_base;
    height_update((*tree)->right);
    height_update(*tree);
}
void right_turn(struct tree **tree){
    struct tree *old_base = *tree;
    *tree = (*tree)->right;
    old_base->right = (*tree)->left;
    (*tree)->left = old_base;
    height_update((*tree)->left);
    height_update(*tree);
}
int inserting(int data,struct tree **pointer){
    struct tree *tree = *pointer;
    if(tree == NULL){
        *pointer = malloc(sizeof(struct tree));
        tree = *pointer;
        tree->data = data;
        tree->height = 1;
        tree->left = NULL;
        tree->right = NULL;
        return 1;
    }
    int height_of_changed_subtree,height_of_other_subtree;
    if(data < tree->data){
        height_of_changed_subtree = inserting(data,&(tree->left));
        if(tree->right)
            height_of_other_subtree = tree->right->height;
        else
            height_of_other_subtree = 0;
        if((height_of_changed_subtree == height_of_other_subtree + 1))
            tree->height = height_of_changed_subtree + 1;
        if(height_of_changed_subtree == height_of_other_subtree + 2){
            if(tree->left->left)
                height_of_changed_subtree = tree->left->left->height;
            else
                height_of_changed_subtree = 0;
            if(tree->left->right)
                height_of_other_subtree = tree->left->right->height;
            else
                height_of_other_subtree = 0;
            if(height_of_changed_subtree <= height_of_other_subtree)
                right_turn(&(tree->left));
            left_turn(pointer);
        }
    }
    else{
        height_of_changed_subtree = inserting(data,&(tree->right));
        if(tree->left)
            height_of_other_subtree = tree->left->height;
        else
            height_of_other_subtree = 0;
        if((height_of_changed_subtree == height_of_other_subtree + 1))
            tree->height = height_of_changed_subtree + 1;
        if(height_of_changed_subtree == height_of_other_subtree + 2){
            if(tree->right->right)
                height_of_changed_subtree = tree->right->right->height;
            else
                height_of_changed_subtree = 0;
            if(tree->right->left)
                height_of_other_subtree = tree->right->left->height;
            else
                height_of_other_subtree = 0;
            if(height_of_changed_subtree <= height_of_other_subtree)
                left_turn(&(tree->right));
            right_turn(pointer);
        }
    }
    return tree->height;
}
void tree_dtor(struct tree *tree){
    if(tree == NULL)
        return;
    if(tree->left)
        tree_dtor(tree->left);
    if(tree->right)
        tree_dtor(tree->right);
    free(tree);
}
int main(){
    int N;
    scanf("%i",&N);
    int i,data;
    struct tree *tree = NULL;
    for(i = 0;i < N;++ i){
        scanf("%i",&data);
        inserting(data,&tree);
    }
    if(tree)
        printf("%i",tree->height);
    else
        printf("0");
    tree_dtor(tree);
    return 0;
}
